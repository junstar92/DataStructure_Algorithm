#include "AES.hpp"
#include <algorithm>
#include <cassert>
#include <random>
#include <time.h>

#define multiply(a) (((a) << 1) ^ (((a >> 7) & 1) * 0x1b))

AES::AES(size_t keyBits, MODE mode) : Nkey_(keyBits / 32), Nround_(6 + this->Nkey_), mode_(mode)
{
	assert(keyBits == 128 || keyBits == 192 || keyBits == 256);

	if (mode == MODE::CBC) {
		this->iv_.assign(this->Nstate_, 0);
		this->setIV(time(NULL));
	}
}

std::vector<byte> AES::_createState(const std::vector<byte>& block, uint32_t start)
{
	uint8_t last = std::min(this->Nstate_, static_cast<uint8_t>(block.size() - start));
	byte init_val = 0;
	if (last != this->Nstate_)
		init_val = this->Nstate_ - last; // using PKCS#7 padding
	std::vector<byte> ret(this->Nstate_, init_val);

	for (uint8_t i = start; i < start + last; i++) {
		ret[i - start] = block[i];
	}

	return ret;
}

std::vector<byte> AES::_expandKey(const std::vector<byte>& cipherKey, bool verbose)
{
	// expand key : Nstate x (Nround + 1) array
	std::vector<byte> roundKey(this->Nstate_ * (this->Nround_ + 1), 0);

	for (uint8_t i = 0; i < this->Nkey_; i++)
	{
		roundKey[(i * this->Ncol_) + 0] = cipherKey[(i * this->Ncol_) + 0];
		roundKey[(i * this->Ncol_) + 1] = cipherKey[(i * this->Ncol_) + 1];
		roundKey[(i * this->Ncol_) + 2] = cipherKey[(i * this->Ncol_) + 2];
		roundKey[(i * this->Ncol_) + 3] = cipherKey[(i * this->Ncol_) + 3];
	}

	for (uint8_t i = this->Nkey_; i < this->Ncol_ * (this->Nround_ + 1); i++)
	{
		byte Wi_1[4];
		uint8_t i_1 = (i - 1) * 4;

		Wi_1[0] = roundKey[i_1 + 0];
		Wi_1[1] = roundKey[i_1 + 1];
		Wi_1[2] = roundKey[i_1 + 2];
		Wi_1[3] = roundKey[i_1 + 3];

		if (i % this->Nkey_ == 0) {
			// RotWords
			byte temp = Wi_1[0];
			Wi_1[0] = Wi_1[1];
			Wi_1[1] = Wi_1[2];
			Wi_1[2] = Wi_1[3];
			Wi_1[3] = temp;

			// SubBytes
			Wi_1[0] = this->_mappingSBox(Wi_1[0]);
			Wi_1[1] = this->_mappingSBox(Wi_1[1]);
			Wi_1[2] = this->_mappingSBox(Wi_1[2]);
			Wi_1[3] = this->_mappingSBox(Wi_1[3]);

			// XOR Rcon
			Wi_1[0] = Wi_1[0] ^ rcon_[i / this->Nkey_];
		}

		if (this->Nkey_ == 8) // if AES256
		{
			if (i % this->Nkey_ == 4)
			{
				Wi_1[0] = this->_mappingSBox(Wi_1[0]);
				Wi_1[1] = this->_mappingSBox(Wi_1[1]);
				Wi_1[2] = this->_mappingSBox(Wi_1[2]);
				Wi_1[3] = this->_mappingSBox(Wi_1[3]);
			}
		}

		uint8_t i_4 = (i - this->Nkey_) * 4;
		roundKey[(i * this->Ncol_) + 0] = roundKey[i_4 + 0] ^ Wi_1[0];
		roundKey[(i * this->Ncol_) + 1] = roundKey[i_4 + 1] ^ Wi_1[1];
		roundKey[(i * this->Ncol_) + 2] = roundKey[i_4 + 2] ^ Wi_1[2];
		roundKey[(i * this->Ncol_) + 3] = roundKey[i_4 + 3] ^ Wi_1[3];
	}

	if (verbose) {
		for (uint8_t i = 0; i < this->Nround_ + 1; i++) {
			printf("\n----- Round Key %d ----\n", i);

			for (uint8_t r = 0; r < this->Bsize_; r++)
			{
				printf("      ");
				for (uint8_t c = 0; c < this->Ncol_; c++)
				{
					printf("%02X ", roundKey[(i * this->Nstate_) + (c * this->Ncol_) + r]);
				}
				printf("\n");
			}
		}
	}

	return roundKey;
}

void AES::_addRoundKey(std::vector<byte>& state, const std::vector<byte>& roundKey, uint8_t round, bool verbose)
{
	for (uint8_t i = 0; i < this->Nstate_; i++) {
		state[i] ^= roundKey[(this->Nstate_ * round) + i];
	}

	if (verbose) {
		printf("\n----- After Round %d -----\n", round);
		this->_printState(state);
	}
}

void AES::_xor_iv(std::vector<byte>& state, std::vector<byte>& iv, bool verbose)
{
	for (uint8_t i = 0; i < this->Nstate_; i++)
		state[i] ^= iv[i];

	if (verbose) {
		printf("\n----- After XOR with Initialization Value -----\n");
		this->_printState(state);
	}
}

void AES::_encryption(std::vector<byte>& state, const std::vector<byte>& roundKey, bool verbose)
{
	this->_addRoundKey(state, roundKey, 0, verbose);
	for (uint8_t i = 1; i < this->Nround_; i++) {
		this->_subBytes(state, verbose);
		this->_shiftRows(state, verbose);
		this->_mixColumns(state, verbose);
		this->_addRoundKey(state, roundKey, i, verbose);
	}
	this->_subBytes(state, verbose);
	this->_shiftRows(state, verbose);
	this->_addRoundKey(state, roundKey, this->Nround_, verbose);
}

byte AES::_mappingSBox(const byte val)
{
	return this->sbox_[val];
}

void AES::_subBytes(std::vector<byte>& state, bool verbose)
{
	for (uint8_t i = 0; i < this->Nstate_; i++) {
		state[i] = this->_mappingSBox(state[i]);
	}

	if (verbose) {
		printf("\n----- After SubBytes -----\n");
		this->_printState(state);
	}
}

void AES::_shiftRows(std::vector<byte>& state, bool verbose)
{
	byte temp;
	// Rotate row 1
	temp = state[1];
	state[1] = state[5];
	state[5] = state[9];
	state[9] = state[13];
	state[13] = temp;

	// Rotate row 2
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;
	temp = state[6];
	state[6] = state[14];
	state[14] = temp;

	// Roate row 3
	temp = state[3];
	state[3] = state[15];
	state[15] = state[11];
	state[11] = state[7];
	state[7] = temp;

	if (verbose) {
		printf("\n----- After ShiftRows -----\n");
		this->_printState(state);
	}
}

void AES::_mixColumns(std::vector<byte>& state, bool verbose)
{
	std::vector<byte> temp(this->Nstate_, 0);
	const uint8_t fixed[] = { 2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2 };

	for (uint8_t i = 0; i < this->Bsize_; i++) {
		for (uint8_t j = 0; j < this->Ncol_; j++) {
			for (uint8_t k = j * 4; k < (j * 4) + 4; k++) {
				if (fixed[k] == 1) {
					temp[(i * this->Ncol_) + j] ^= state[(i * this->Ncol_) + (k % 4)];
				}
				else {
					temp[(i * this->Ncol_) + j] ^= multiply(state[(i * this->Ncol_) + (k % 4)]);
					if (fixed[k] == 3) {
						temp[(i * this->Ncol_) + j] ^= state[(i * this->Ncol_) + (k % 4)];
					}
				}
			}
		}
	}

	for (uint8_t i = 0; i < this->Nstate_; i++) {
		state[i] = temp[i];
	}

	if (verbose) {
		printf("\n----- After MixColumns -----\n");
		this->_printState(state);
	}
}

void AES::_decryption(std::vector<byte>& state, const std::vector<byte>& roundKey, bool verbose)
{
	this->_addRoundKey(state, roundKey, this->Nround_, verbose);
	for (uint8_t i = this->Nround_ - 1; i > 0; i--) {
		this->_invShiftRows(state, verbose);
		this->_invSubBytes(state, verbose);
		this->_addRoundKey(state, roundKey, i, verbose);
		this->_invMixColumns(state, verbose);
	}
	this->_invShiftRows(state, verbose);
	this->_invSubBytes(state, verbose);
	this->_addRoundKey(state, roundKey, 0, verbose);
}

byte AES::_mappingInvSBox(const byte val)
{
	return this->inv_sbox_[val];
}

void AES::_invSubBytes(std::vector<byte>& state, bool verbose)
{
	for (uint8_t i = 0; i < this->Nstate_; i++) {
		state[i] = this->_mappingInvSBox(state[i]);
	}

	if (verbose) {
		printf("\n----- After Inverse SubBytes -----\n");
		this->_printState(state);
	}
}

void AES::_invShiftRows(std::vector<byte>& state, bool verbose)
{
	byte temp;
	// Rotate row 1
	temp = state[1];
	state[1] = state[13];
	state[13] = state[9];
	state[9] = state[5];
	state[5] = temp;

	// Rotate row 2
	temp = state[2];
	state[2] = state[10];
	state[10] = temp;

	temp = state[6];
	state[6] = state[14];
	state[14] = temp;

	// Roate row 3
	temp = state[3];
	state[3] = state[7];
	state[7] = state[11];
	state[11] = state[15];
	state[15] = temp;

	if (verbose) {
		printf("\n----- After Inverse ShiftRows -----\n");
		this->_printState(state);
	}
}

void AES::_invMixColumns(std::vector<byte>& state, bool verbose)
{
	std::vector<byte> temp(this->Nstate_, 0);
	const uint8_t fixed[] = { 14, 11, 13, 9, 9, 14, 11, 13, 13, 9 ,14, 11, 11, 13, 9, 14 };

	for (uint8_t i = 0; i < this->Bsize_; i++) {
		for (uint8_t j = 0; j < this->Ncol_; j++) {
			for (uint8_t k = j * 4; k < (j * 4) + 4; k++) {
				temp[(i * this->Ncol_) + j] ^= multiply(multiply(multiply(state[(i * this->Ncol_) + (k % 4)])));

				if (fixed[k] == 9 || fixed[k] == 11 || fixed[k] == 13) {
					temp[(i * this->Ncol_) + j] ^= state[(i * this->Ncol_) + (k % 4)];
				}
				if (fixed[k] == 11 || fixed[k] == 14) {
					temp[(i * this->Ncol_) + j] ^= multiply(state[(i * this->Ncol_) + (k % 4)]);
				}
				if (fixed[k] == 13 || fixed[k] == 14) {
					temp[(i * this->Ncol_) + j] ^= multiply(multiply(state[(i * this->Ncol_) + (k % 4)]));
				}
			}
		}
	}

	for (uint8_t i = 0; i < this->Nstate_; i++) {
		state[i] = temp[i];
	}

	if (verbose) {
		printf("\n----- After Inverse MixColumns -----\n");
		this->_printState(state);
	}
}

std::string AES::_convertTypeByteStateToStr(const std::vector<byte>& state, bool decryption)
{
	std::string ret = "";

	char buf[3];
	for (uint32_t i = 0; i < this->Nstate_; i++) {
		snprintf(buf, sizeof(buf), "%02X", state[i]);

		if (decryption) { // check padding value
			if ((this->Nstate_ - i) == state[i]) {
				byte padding = state[i];
				uint32_t j;
				for (j = i + 1; j < this->Nstate_; j++) {
					if (state[j] != padding)
						break;
				}
				if (j == this->Nstate_) // if all remained bytes are same with padding, stop converting
					return ret;
			}
		}

		ret += buf;
	}

	return ret;
}

std::vector<byte> AES::_convertTypeStrToByteBlock(std::string str)
{
	uint32_t len = str.length();
	std::vector<byte> block(len / 2, 0);

	// to upper
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	byte tmp = 0;
	for (uint32_t i = 0; i < len; i++) {
		if (str[i] >= '0' && str[i] <= '9') {
			tmp = 16 * tmp + (str[i] - '0');
		}
		else if (str[i] >= 'A' && str[i] <= 'F') {
			tmp = 16 * tmp + (str[i] - 'A' + 10);
		}
		else {
			return std::vector<byte>();
		}

		if (i % 2 == 1) {
			block[i / 2] = tmp;
			tmp = 0;
		}
	}

	return block;
}

void AES::_printState(const std::vector<byte>& state)
{
	for (uint8_t r = 0; r < this->Bsize_; r++)
	{
		printf("      ");
		for (uint8_t c = 0; c < this->Ncol_; c++)
		{
			printf("%02X ", state[(c * this->Ncol_) + r]);
		}
		printf("\n");
	}
}

void AES::setIV(uint32_t seed)
{
	assert(this->mode_ == MODE::CBC);
	srand(seed);

	for (uint8_t i = 0; i < this->Nstate_; i++)
		this->iv_[i] = rand() % 256;
}

void AES::setIV(std::vector<byte> iv)
{
	assert(this->mode_ == MODE::CBC);
	assert(iv.size() == 16);

	for (uint8_t i = 0; i < this->Nstate_; i++)
		this->iv_[i] = iv[i];
}

std::vector<byte> AES::getIV() const
{
	return this->iv_;
}

std::string AES::encryption(const std::string& input, const std::string& cipherKey, bool verbose)
{
	assert(cipherKey.size() == this->Nkey_ * 4 * 2); // check cipherKey(str) size == 128 / 192 / 256 bits * 2

	uint32_t block_size = input.length() / (this->Nstate_ * 2);
	if (input.length() % (this->Nstate_ * 2) != 0)
		block_size += 1;

	std::string cipherText = "";
	std::vector<byte> plainText = this->_convertTypeStrToByteBlock(input);
	std::vector<byte> _cipherKey = this->_convertTypeStrToByteBlock(cipherKey);
	std::vector<byte> roundKey = this->_expandKey(_cipherKey, verbose);

	if (this->mode_ == MODE::CBC) {
		this->phiv_ = this->iv_;
	}

	for (uint32_t i = 0; i < block_size; i++) {
		std::vector<byte> state = this->_createState(plainText, i * this->Nstate_);

		if (this->mode_ == MODE::CBC) {
			this->_xor_iv(state, this->phiv_, verbose);
		}

		this->_encryption(state, roundKey, verbose);

		if (this->mode_ == MODE::CBC) {
			this->phiv_ = state;
		}

		cipherText += this->_convertTypeByteStateToStr(state, false);
	}

	if (verbose) {
		std::cout << "\n --- Cipher Text --- \n";
		uint32_t i = 0;
		for (auto& c : cipherText) {
			std::cout << c;
			i++;
			if (i % (this->Nstate_ * 2) == 0)
				std::cout << "\n";
		}
		std::cout << "\n";
	}

	return cipherText;
}

std::string AES::decryption(const std::string& input, const std::string& cipherKey, bool verbose)
{
	assert(cipherKey.size() == this->Nkey_ * 4 * 2); // check cipherKey(str) size == 128 / 192 / 256 bits * 2

	uint32_t block_size = input.length() / (this->Nstate_ * 2);

	std::string plainText = "";
	std::vector<byte> cipherText = this->_convertTypeStrToByteBlock(input);
	std::vector<byte> _cipherKey = this->_convertTypeStrToByteBlock(cipherKey);
	std::vector<byte> roundKey = this->_expandKey(_cipherKey, verbose);

	if (this->mode_ == MODE::CBC) {
		this->phiv_ = this->iv_;
	}

	for (uint32_t i = 0; i < block_size; i++) {
		std::vector<byte> state = this->_createState(cipherText, i * this->Nstate_);
		std::vector<byte> tmp;

		if (this->mode_ == MODE::CBC) {
			tmp = state;
		}

		this->_decryption(state, roundKey, verbose);

		if (this->mode_ == MODE::CBC) {
			this->_xor_iv(state, this->phiv_, verbose);
			this->phiv_ = tmp;
		}

		plainText += this->_convertTypeByteStateToStr(state, true);
	}

	if (verbose) {
		std::cout << "\n --- Plain Text --- \n";
		uint32_t i = 0;
		for (auto& c : plainText) {
			std::cout << c;
			i++;
			if (i % (this->Nstate_ * 2) == 0)
				std::cout << "\n";
		}
		std::cout << "\n";
	}

	return plainText;
}

std::string AES::_convertCharToStrHex(const char& c)
{
	std::string ret = "";

	uint8_t temp = c / 16;
	if (temp >= 0 && temp <= 9)
		ret += ('0' + temp);
	else
		ret += ('A' + temp - 10);

	temp = c % 16;
	if (temp >= 0 && temp <= 9)
		ret += ('0' + temp);
	else
		ret += ('A' + temp - 10);

	return ret;
}

std::string AES::convertStrToHexStr(const std::string& str)
{
	std::string ret;

	for (auto& c : str) {
		ret += _convertCharToStrHex(c);
	}

	return ret;
}

std::string AES::convertHexStrToStr(const std::string& hex)
{
	std::string ret;

	for (uint32_t i = 0; i < hex.length(); i += 2) {
		byte tmp = 0;
		if (hex[i] >= '0' && hex[i] <= '9')
			tmp += (hex[i] - '0');
		else
			tmp += (hex[i] - 'A' + 10);
		tmp *= 16;
		if (hex[i + 1] >= '0' && hex[i + 1] <= '9')
			tmp += (hex[i + 1] - '0');
		else
			tmp += (hex[i + 1] - 'A' + 10);

		if (tmp == 0)
			break;

		ret += static_cast<char>(tmp);
	}

	return ret;
}