// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)throw "negative length";
	BitLen = len;
	MemLen = len / sz + (len % sz > 0 ? 1 : 0);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen / sz;
	delete[] pMem;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)throw "out of range";
	return BitLen / sz;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)throw "out of range";
	return 1 << (n % sz);
}


// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0 || n>=BitLen)throw "out of range";
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0 || n>=BitLen)throw "out of range";
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0 || n>=BitLen)throw "out of range";
	return pMem[GetMemIndex(n)] & GetMemMask(n) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (bf != *this) {
		if (MemLen == bf.MemLen)for (int i = 0; i < MemLen; i++)pMem[i] = bf.pMem[i];
		else {
			delete[] pMem;
			BitLen = bf.BitLen;
			MemLen = bf.BitLen / sz;
			pMem = new TELEM[MemLen];
			for (int i = 0; i < MemLen; i++)pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.GetLength())return 0;
	for (int i = 0; i < MemLen; i++)if (pMem[i] != bf.pMem[i])return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(bf == *this);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (bf.GetLength() != BitLen)throw "lengths do not match";
	TBitField out = TBitField(BitLen);
	for (int i = 0; i < MemLen; i++)out.pMem[i] = pMem[i] | bf.pMem[i];
	return out;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (bf.GetLength() != BitLen)throw "lengths do not match";
	TBitField out = TBitField(BitLen);
	for (int i = 0; i < MemLen; i++)out.pMem[i] = pMem[i] & bf.pMem[i];
	return out;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField t(BitLen);
	for (int i = 0; i < MemLen; i++)t.pMem[i] = ~pMem[i];
	return t;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int a;
	while (istr >> a)bf.SetBit(a);
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++) {
		if (bf.GetBit(i))ostr << i<<" ";
	}
	return ostr;
}
