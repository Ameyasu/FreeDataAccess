#pragma once

#include "DataFormat.h"
#include <type_traits>
#include <memory>
#include <string>

/// <summary>
/// <para>���I�Ɍ^�ύX�\(�v���~�e�B�u�^)�ȕϐ���\������N���X</para>
/// <para>�^�̃T�C�Y���E�z��̗v�f���E�l���i�[���郁�����ւ̃|�C���^ �Ȃǂ���������</para>
/// <para>���̃N���X�ɂ����Č^�̃T�C�Y�Ƃ����̂̓|�C���^����菜�����^�̃T�C�Y��\��</para>
/// <para>char�^�̔z���ݒ肷��Ƃ��́A�K���k���I�[������ł��邱��</para>
/// </summary>
class DataItem
{
public:
	/// <summary>
	/// <para>DataItem�N���X�������̏����^�C�v�̃f�t�H���g�l��ݒ肷��</para>
	/// </summary>
	/// <param name="format">AUTO=�^�ɂ����������^�C�v, HEX=�ǂ�Ȍ^�ł�HEX</param>
	static void setDefaultDataFormat(DefaultDataFormat format);

public:
	/// <summary>
	/// <para>DataBox�N���X�Ɏg����d�g�ݏ�d���Ȃ��p�ӂ��Ă���</para>
	/// <para>���[�U�[�͎g��Ȃ����ƁI</para>
	/// </summary>
	DataItem();

	/// <summary>
	/// <para>�D���Ȓl��ݒ肷��</para>
	/// </summary>
	template<typename T>
	DataItem(T element);

	/// <summary>
	/// <para>�z���ݒ肷��</para>
	/// <para>Shallow�R�s�[����ꍇ�A���L���͂��̃N���X������������delete�����</para>
	/// </summary>
	/// <param name="elementPointer">�z��̐擪�̃|�C���^</param>
	/// <param name="elementCount">�z��̗v�f��</param>
	/// <param name="deepCopy">true=Deep�R�s�[(default), false=Shallow�R�s�[</param>
	template<typename T>
	DataItem(T* elementPointer, size_t elementCount, bool deepCopy = true);

	/// <summary>
	/// <para>�z���ݒ肷��</para>
	/// <para>const�|�C���^��n�����ꍇ�A���̃R���X�g���N�^���Ă΂�K��Deep�R�s�[�ɂȂ�</para>
	/// </summary>
	/// <param name="elementPointer">�z��̐擪�̃|�C���^</param>
	/// <param name="elementCount">�z��̗v�f��</param>
	template<typename T>
	DataItem(const T* elementPointer, size_t elementCount);

	/// <summary>
	/// <para>�k���I�[�������ݒ肷��</para>
	/// </summary>
	DataItem(const char* text);

	/// <summary>
	/// <para>Deep�R�s�[</para>
	/// </summary>
	DataItem(const DataItem& rhs);

	/// <summary>
	/// <para>Deep�R�s�[</para>
	/// </summary>
	DataItem& operator=(const DataItem& rhs);

	DataItem(DataItem&& rhs) noexcept;
	DataItem& operator=(DataItem&& rhs) noexcept;

	~DataItem();

public:
	/// <summary>
	/// <para>DataItem�̏�Ԓl�����������񂩂�A���̏�Ԓl��DataItem�𐶐�����</para>
	/// <para>�������Ԉ���Ă���Ɨ�O</para>
	/// </summary>
	static DataItem createFromFormat(const char* format);

public:
	/// <returns>���g�̏�Ԓl������������</returns>
	const char* operator()() const;

	/// <summary>
	/// <para>�L���X�g���邱�Ƃŕێ����Ă���l��m�邱�Ƃ��ł���</para>
	/// <para>�ݒ肵���^�ƈقȂ�T�C�Y�̌^�ɃL���X�g���悤�Ƃ���Ɨ�O</para>
	/// </summary>
	template<typename T>
	operator T() const;

	/// <summary>
	/// <para>�L���X�g���邱�Ƃŕێ����Ă���l��m�邱�Ƃ��ł���</para>
	/// <para>�ݒ肵���^�ƈقȂ�T�C�Y�̌^�ɃL���X�g���悤�Ƃ���Ɨ�O</para>
	/// </summary>
	template<typename T>
	operator T* () const;

	/// <summary>
	/// <para>�ێ����Ă���l���㏑������</para>
	/// <para>�ݒ肵���^�ƈقȂ�T�C�Y�̌^�������悤�Ƃ���Ɨ�O</para>
	/// </summary>
	template<typename T>
	void operator=(T element);

	/// <summary>
	/// <para>�ێ����Ă���l���^��񂲂Ə㏑������</para>
	/// <para>�����^�C�v�̓f�t�H���g�l�ɕύX�����</para>
	/// </summary>
	template<typename T>
	void operator<<(T element);

	/// <summary>
	/// <para>�ێ����Ă���l���^��񂲂Ə㏑������</para>
	/// <para>�����^�C�v�̓f�t�H���g�l�ɕύX�����</para>
	/// <para>Shallow�R�s�[����</para>
	/// <para>���L���͂��̃N���X�������A������delete�����</para>
	/// </summary>
	/// <param name="elementPointer">�z��̐擪�̃|�C���^</param>
	/// <param name="elementCount">�z��̗v�f��</param>
	template<typename T>
	void shallow(T* elementPointer, size_t elementCount);

	/// <summary>
	/// <para>�ێ����Ă���l���^��񂲂Ə㏑������</para>
	/// <para>�����^�C�v�̓f�t�H���g�l�ɕύX�����</para>
	/// <para>Deep�R�s�[����</para>
	/// </summary>
	/// <param name="elementPointer">�z��̐擪�̃|�C���^</param>
	/// <param name="elementCount">�z��̗v�f��</param>
	template<typename T>
	void deep(const T* elementPointer, size_t elementCount);

	/// <returns>�ݒ肳��Ă���^�̃T�C�Y</returns>
	size_t getElementSize() const;

	/// <returns>
	/// <para>�ݒ肳��Ă���z��̗v�f��</para>
	/// <para>0=�z��łȂ�, 1�ȏ�=�z��̗v�f��</para>
	/// </returns>
	size_t getElementCount() const;

	/// <returns>���g�̏�Ԓl������������̏����^�C�v</returns>
	DataFormat getFormat() const;

	/// <summary>
	/// <para>���g�̏�Ԓl������������̏����^�C�v��ݒ肷��</para>
	/// <para>�ݒ肳��Ă���^�ɔ�Ή��̏����^�C�v��ݒ肷��Ɨ�O</para>
	/// </summary>
	/// <param name="format">�����^�C�v</param>
	void setFormat(DataFormat format);

private:
	void deleteData();

	template<typename T>
	DataFormat getDefaultFormat();

private:
	static DefaultDataFormat ms_defaultFormat;

private:
	size_t m_elementSize;
	size_t m_elementCount;
	void* m_elementPointer;

	DataFormat m_format;
	mutable std::string m_text;
	mutable bool m_cache;
};




DefaultDataFormat DataItem::ms_defaultFormat = DefaultDataFormat::AUTO;

inline void DataItem::setDefaultDataFormat(DefaultDataFormat format)
{
	ms_defaultFormat = format;
}

inline DataItem::DataItem()
	: m_elementSize()
	, m_elementCount()
	, m_elementPointer()
	, m_format()
	, m_text()
	, m_cache()
{}

template<typename T>
inline DataItem::DataItem(T element)
	: m_elementSize(std::alignment_of_v<T>)
	, m_elementCount(0)
	, m_elementPointer()
	, m_format(getDefaultFormat<T>())
	, m_text()
	, m_cache()
{
	static_assert(!std::is_pointer_v<T> && !std::is_array_v<T>, "�|�C���^�E�z��͖���");
	m_elementPointer = new T(element);
}

template<typename T>
inline DataItem::DataItem(T* elementPointer, size_t elementCount, bool deepCopy)
	: m_elementSize(std::alignment_of_v<T>)
	, m_elementCount(elementCount)
	, m_elementPointer()
	, m_format(getDefaultFormat<T>())
	, m_text()
	, m_cache()
{
	if (deepCopy)
	{
		m_elementPointer = new T[elementCount];
		memcpy(m_elementPointer, elementPointer, m_elementSize * elementCount);
	}
	else
	{
		m_elementPointer = elementPointer;
	}
}

template<typename T>
inline DataItem::DataItem(const T* elementPointer, size_t elementCount)
	: m_elementSize(std::alignment_of_v<T>)
	, m_elementCount(elementCount)
	, m_elementPointer()
	, m_format(getDefaultFormat<T>())
	, m_text()
	, m_cache()
{
	m_elementPointer = new T[elementCount];
	memcpy(m_elementPointer, elementPointer, m_elementSize * elementCount);
}

inline DataItem::DataItem(const char* text)
	: m_elementSize(std::alignment_of_v<char>)
	, m_elementCount()
	, m_elementPointer()
	, m_format(getDefaultFormat<char>())
	, m_text()
	, m_cache()
{
	size_t c = 0;
	while (text[c] != '\0') ++c;
	++c;

	m_elementCount = c;
	m_elementPointer = new char[c];
	memcpy(m_elementPointer, text, m_elementSize * c);
}

inline DataItem::DataItem(const DataItem& rhs)
	: m_elementSize(rhs.m_elementSize)
	, m_elementCount(rhs.m_elementCount)
	, m_elementPointer()
	, m_format(rhs.m_format)
	, m_text(rhs.m_text)
	, m_cache(rhs.m_cache)
{
	if (m_elementCount == 0)
	{
		switch (m_elementSize)
		{
		case 1: m_elementPointer = new uint8_t(*static_cast<uint8_t*>(rhs.m_elementPointer)); break;
		case 2: m_elementPointer = new uint16_t(*static_cast<uint16_t*>(rhs.m_elementPointer)); break;
		case 4: m_elementPointer = new uint32_t(*static_cast<uint32_t*>(rhs.m_elementPointer)); break;
		case 8: m_elementPointer = new uint64_t(*static_cast<uint64_t*>(rhs.m_elementPointer)); break;
		default: throw;
		}
	}
	else
	{
		switch (m_elementSize)
		{
		case 1: m_elementPointer = new uint8_t[m_elementCount]; break;
		case 2: m_elementPointer = new uint16_t[m_elementCount]; break;
		case 4: m_elementPointer = new uint32_t[m_elementCount]; break;
		case 8: m_elementPointer = new uint64_t[m_elementCount]; break;
		default: throw;
		}
		memcpy(m_elementPointer, rhs.m_elementPointer, m_elementSize * m_elementCount);
	}
}

inline DataItem& DataItem::operator=(const DataItem& rhs)
{
	deleteData();

	m_elementSize = rhs.m_elementSize;
	m_elementCount = rhs.m_elementCount;
	m_format = rhs.m_format;
	m_text = rhs.m_text;
	m_cache = rhs.m_cache;

	if (m_elementCount == 0)
	{
		switch (m_elementSize)
		{
		case 1: m_elementPointer = new uint8_t(*static_cast<uint8_t*>(rhs.m_elementPointer)); break;
		case 2: m_elementPointer = new uint16_t(*static_cast<uint16_t*>(rhs.m_elementPointer)); break;
		case 4: m_elementPointer = new uint32_t(*static_cast<uint32_t*>(rhs.m_elementPointer)); break;
		case 8: m_elementPointer = new uint64_t(*static_cast<uint64_t*>(rhs.m_elementPointer)); break;
		default: throw;
		}
	}
	else
	{
		switch (m_elementSize)
		{
		case 1: m_elementPointer = new uint8_t[m_elementCount]; break;
		case 2: m_elementPointer = new uint16_t[m_elementCount]; break;
		case 4: m_elementPointer = new uint32_t[m_elementCount]; break;
		case 8: m_elementPointer = new uint64_t[m_elementCount]; break;
		default: throw;
		}
		memcpy(m_elementPointer, rhs.m_elementPointer, m_elementSize * m_elementCount);
	}

	return *this;
}

inline DataItem::DataItem(DataItem&& rhs) noexcept
	: m_elementSize(rhs.m_elementSize)
	, m_elementCount(rhs.m_elementCount)
	, m_elementPointer(rhs.m_elementPointer)
	, m_format(rhs.m_format)
	, m_text(std::move(rhs.m_text))
	, m_cache(rhs.m_cache)
{
	rhs.m_elementPointer = nullptr;
}

inline DataItem& DataItem::operator=(DataItem&& rhs) noexcept
{
	deleteData();

	m_elementSize = rhs.m_elementSize;
	m_elementCount = rhs.m_elementCount;
	m_elementPointer = rhs.m_elementPointer;
	m_format = rhs.m_format;
	m_text = std::move(rhs.m_text);
	m_cache = rhs.m_cache;

	rhs.m_elementPointer = nullptr;
	return *this;
}

inline DataItem::~DataItem()
{
	deleteData();
}

inline DataItem DataItem::createFromFormat(const char* format)
{
	DataItem item;
	item.m_text = format;
	item.m_cache = true;

	// HEX
	if (format[0] == '0' && format[1] == 'x')
	{
		const char* p = format + 2;
		int s = 0;
		uint64_t v = 0;
		while (p[0] != '\0') {
			v <<= 4;
			if ('0' <= p[0] && p[0] <= '9')
				v += p[0] - '0';
			else if ('A' <= p[0] && p[0] <= 'F')
				v += p[0] - 'A' + 10;
			else
				throw;
			++p;
			++s;
		}
		s >>= 1;
		switch (s)
		{
		case 1: item.m_elementPointer = new uint8_t(static_cast<uint8_t>(v)); break;
		case 2: item.m_elementPointer = new uint16_t(static_cast<uint16_t>(v)); break;
		case 4: item.m_elementPointer = new uint32_t(static_cast<uint32_t>(v)); break;
		case 8: item.m_elementPointer = new uint64_t(static_cast<uint64_t>(v)); break;
		default: throw;
		}
		item.m_elementSize = s;
		return item;
	}

	// HEX array
	if (format[0] == '{' && format[1] == '0' && format[2] == 'x')
	{
		int c = 1;
		int s = 0;
		{
			const char* p = format + 3;
			while (true)
			{
				if (p[0] == ',')
					++c;
				else if (p[0] == '}')
					break;
				else if (p[0] == '\0')
					throw;
				++p;
			}
			p = format + 3;
			while (('0' <= p[0] && p[0] <= '9') || ('A' <= p[0] && p[0] <= 'F'))
			{
				++p;
				++s;
			}
			s >>= 1;
		}
		item.m_elementCount = c;
		item.m_elementSize = s;
		switch (s)
		{
		case 1: item.m_elementPointer = new uint8_t[c]; break;
		case 2: item.m_elementPointer = new uint16_t[c]; break;
		case 4: item.m_elementPointer = new uint32_t[c]; break;
		case 8: item.m_elementPointer = new uint64_t[c]; break;
		default: throw;
		}

		const char* p = format + 3;
		for (int i = 0; i < c; ++i)
		{
			uint64_t v = 0;
			while (true) {
				if ('0' <= p[0] && p[0] <= '9')
				{
					v <<= 4;
					v += p[0] - '0';
				}
				else if ('A' <= p[0] && p[0] <= 'F')
				{
					v <<= 4;
					v += p[0] - 'A' + 10;
				}
				else if (p[0] == ',' && p[1] == '0' && p[2] == 'x')
				{
					p += 3;
					break;
				}
				else if ((p[0] == '}') && (i == c - 1))
					break;
				else
					throw;
				++p;
			}

			switch (s)
			{
			case 1: static_cast<uint8_t*>(item.m_elementPointer)[i] = static_cast<uint8_t>(v); break;
			case 2: static_cast<uint16_t*>(item.m_elementPointer)[i] = static_cast<uint16_t>(v); break;
			case 4: static_cast<uint32_t*>(item.m_elementPointer)[i] = static_cast<uint32_t>(v); break;
			case 8: static_cast<uint64_t*>(item.m_elementPointer)[i] = static_cast<uint64_t>(v); break;
			}
		}
		return item;
	}

	// REAL
	if (format[0] == '$' || ('0' <= format[0] && format[0] <= '9') || format[0] == '-')
	{
		const char* p = format;
		if (format[0] == '$')
		{
			item.m_elementSize = sizeof(float);
			item.m_elementPointer = new float(static_cast<float>(atof(format + 1)));
		}
		else
		{
			item.m_elementSize = sizeof(double);
			item.m_elementPointer = new double(atof(format));
		}
		item.m_format = DataFormat::REAL;
		return item;
	}

	// REAL array
	if (format[0] == '{' && (format[1] == '$' || ('0' <= format[1] && format[1] <= '9') || format[1] == '-'))
	{
		bool f = format[1] == '$';

		int c = 1;
		{
			const char* p = format + (f ? 2 : 1);
			while (true)
			{
				if (p[0] == ',')
					++c;
				else if (p[0] == '}')
					break;
				else if (p[0] == '\0')
					throw;
				++p;
			}
		}
		item.m_elementCount = c;
		if (f)
		{
			item.m_elementSize = sizeof(float);
			item.m_elementPointer = new float[c];

			const char* p = format + 2;
			for (int i = 0; i < c; ++i)
			{
				static_cast<float*>(item.m_elementPointer)[i] = static_cast<float>(atof(p));
				while (true)
				{
					++p;
					if (p[0] == ',' && p[1] == '$')
					{
						p += 2;
						break;
					}
					else if ((p[0] == '}') && (i == c - 1))
						break;
					else if (p[0] == '\0')
						throw;
				}
			}
		}
		else
		{
			item.m_elementSize = sizeof(double);
			item.m_elementPointer = new double[c];

			const char* p = format + 1;
			for (int i = 0; i < c; ++i)
			{
				static_cast<double*>(item.m_elementPointer)[i] = atof(p);
				while (true)
				{
					++p;
					if (p[0] == ',')
					{
						++p;
						break;
					}
					else if ((p[0] == '}') && (i == c - 1))
						break;
					else if (p[0] == '\0')
						throw;
				}
			}
		}
		item.m_format = DataFormat::REAL;
		return item;
	}

	// BOOL
	if (format[0] == 'f' || format[0] == 't')
	{
		item.m_elementSize = sizeof(bool);
		item.m_elementPointer = new bool(format[0] == 't');
		item.m_format = DataFormat::BOOL;
		return item;
	}

	// BOOL array
	if (format[0] == '{' && (format[1] == 't' || format[1] == 'f'))
	{
		int c = 1;
		{
			const char* p = format;
			while (true)
			{
				if (p[0] == ',')
					++c;
				else if (p[0] == '}')
					break;
				else if (p[0] == '\0')
					throw;
				++p;
			}
		}
		item.m_elementSize = sizeof(bool);
		item.m_elementCount = c;
		item.m_elementPointer = new bool[c];
		item.m_format = DataFormat::BOOL;

		const char* p = format + 1;
		for (int i = 0; i < c; ++i)
		{
			static_cast<bool*>(item.m_elementPointer)[i] = p[0] == 't';
			while (true)
			{
				++p;
				if (p[0] == ',')
				{
					++p;
					break;
				}
				else if ((p[0] == '}') && (i == c - 1))
					break;
				else if (p[0] == '\0')
					throw;
			}
		}
		return item;
	}

	// TEXT char
	if (format[0] == '\'')
	{
		item.m_elementSize = sizeof(char);
		item.m_elementPointer = new char(format[1]);
		item.m_format = DataFormat::TEXT;
		return item;
	}

	// TEXT char array
	if (format[0] == '{' && format[1] == '\'')
	{
		int c = 1;
		{
			const char* p = format;
			while (true)
			{
				if (p[0] == ',')
					++c;
				else if (p[0] == '}')
					break;
				else if (p[0] == '\0')
					throw;
				++p;
			}
		}
		item.m_elementSize = sizeof(char);
		item.m_elementCount = c;
		item.m_elementPointer = new char[c];
		item.m_format = DataFormat::TEXT;

		const char* p = format + 1;
		for (int i = 0; i < c; ++i)
		{
			if (p[0] == '\0' || p[1] == '\0' || p[2] == '\0' || p[3] == '\0')
				throw;

			if (p[0] == '\'' && p[2] == '\'')
			{
				static_cast<char*>(item.m_elementPointer)[i] = p[1];

				if (p[3] == ',')
					p += 4;
				else if (p[3] != '}')
					throw;
			}
		}
		return item;
	}

	// TEXT string
	if (format[0] == '\"')
	{
		int c = 1;
		{
			const char* p = format + 1;
			while (true)
			{
				if (p[0] == '\"')
					break;
				else if (p[0] == '\0')
					throw;
				++p;
				++c;
			}
		}
		item.m_elementSize = sizeof(char);
		item.m_elementCount = c;
		item.m_elementPointer = new char[c];
		item.m_format = DataFormat::TEXT;

		if (c >= 2)
			memcpy(item.m_elementPointer, format + 1, sizeof(char) * (c - 1));

		static_cast<char*>(item.m_elementPointer)[c - 1] = '\0';

		return item;
	}

	throw;
	return item;
}

inline const char* DataItem::operator()() const
{
	if (m_cache)
		return m_text.c_str();

	m_cache = true;

	char buf[32] = {};
	switch (m_format)
	{
	case DataFormat::HEX:
	{
		if (m_elementCount == 0)
		{
			switch (m_elementSize)
			{
			case 1: sprintf_s(buf, "0x%02X", *static_cast<uint8_t*>(m_elementPointer)); break;
			case 2: sprintf_s(buf, "0x%04X", *static_cast<uint16_t*>(m_elementPointer)); break;
			case 4: sprintf_s(buf, "0x%08X", *static_cast<uint32_t*>(m_elementPointer)); break;
			case 8: sprintf_s(buf, "0x%016llX", *static_cast<uint64_t*>(m_elementPointer)); break;
			default: throw;
			}
			m_text = buf;
		}
		else
		{
			struct Fnc
			{
				void* ep;
				char(&buf)[32];
				void byte1(int c) { sprintf_s(buf, "0x%02X,", static_cast<uint8_t*>(ep)[c]); }
				void byte2(int c) { sprintf_s(buf, "0x%04X,", static_cast<uint16_t*>(ep)[c]); }
				void byte4(int c) { sprintf_s(buf, "0x%08X,", static_cast<uint32_t*>(ep)[c]); }
				void byte8(int c) { sprintf_s(buf, "0x%016llX,", static_cast<uint64_t*>(ep)[c]); }
			} fnc = {m_elementPointer, buf};
			void(Fnc::*fncP)(int c) = nullptr;
			switch (m_elementSize)
			{
			case 1: fncP = &Fnc::byte1; break;
			case 2: fncP = &Fnc::byte2; break;
			case 4: fncP = &Fnc::byte4; break;
			case 8: fncP = &Fnc::byte8; break;
			default: throw;
			}
			m_text = "{";
			for (int c = 0; c < m_elementCount; ++c)
			{
				(fnc.*fncP)(c);
				m_text.append(buf);
			}
			m_text[m_text.size() - 1] = '}';
		}
	}
	break;
	case DataFormat::REAL:
	{
		if (m_elementCount == 0)
		{
			switch (m_elementSize)
			{
			case sizeof(float): sprintf_s(buf, "$%.15g", *static_cast<float*>(m_elementPointer)); break;
			case sizeof(double): sprintf_s(buf, "%.15g", *static_cast<double*>(m_elementPointer)); break;
			default: throw;
			}
			
			m_text = buf;
		}
		else
		{
			struct Fnc
			{
				void* ep;
				char(&buf)[32];
				void f(int c) { sprintf_s(buf, "$%.15g,", static_cast<float*>(ep)[c]); }
				void d(int c) { sprintf_s(buf, "%.15g,", static_cast<double*>(ep)[c]); }
			} fnc = {m_elementPointer, buf};
			void(Fnc:: * fncP)(int c) = nullptr;
			switch (m_elementSize)
			{
			case sizeof(float): fncP = &Fnc::f; break;
			case sizeof(double): fncP = &Fnc::d; break;
			default: throw;
			}
			m_text = "{";
			for (int c = 0; c < m_elementCount; ++c)
			{
				(fnc.*fncP)(c);
				m_text.append(buf);
			}
			m_text[m_text.size() - 1] = '}';
		}
	}
	break;
	case DataFormat::BOOL:
	{
		if (m_elementSize != 1)
			throw;

		if (m_elementCount == 0)
		{
			sprintf_s(buf, "%s", *static_cast<bool*>(m_elementPointer) ? "true" : "false");
			m_text = buf;
		}
		else
		{
			m_text = "{";
			for (int c = 0; c < m_elementCount; ++c)
			{
				sprintf_s(buf, "%s,", static_cast<bool*>(m_elementPointer)[c] ? "true" : "false");
				m_text.append(buf);
			}
			m_text[m_text.size() - 1] = '}';
		}
	}
	break;
	case DataFormat::TEXT:
	{
		if (m_elementSize != 1)
			throw;

		if (m_elementCount == 0)
		{
			m_text = '\'';
			m_text += *static_cast<char*>(m_elementPointer);
			m_text += '\'';
		}
		else
		{
			if (static_cast<char*>(m_elementPointer)[m_elementCount - 1] == '\0')
			{
				m_text = '\"';
				m_text.append(static_cast<char*>(m_elementPointer));
				m_text.append("\"");
			}
			else
			{
				m_text = "{";
				for (int c = 0; c < m_elementCount; ++c)
				{
					sprintf_s(buf, "\'%c\',", static_cast<char*>(m_elementPointer)[c]);
					m_text.append(buf);
				}
				m_text[m_text.size() - 1] = '}';
			}
		}
	}
	break;
	default:
		throw;
	}
	return m_text.c_str();
}

template<typename T>
inline DataItem::operator T() const
{
	static_assert(!std::is_pointer_v<T> && !std::is_array_v<T>, "�|�C���^�E�z��͖���");

	if (std::alignment_of_v<T> != m_elementSize)
		throw;

	return *static_cast<T*>(m_elementPointer);
}

template<typename T>
inline DataItem::operator T* () const
{
	static_assert(!std::is_pointer_v<T> && !std::is_array_v<T>, "�|�C���^�E�z��͖���");

	if (std::alignment_of_v<T> != m_elementSize)
		throw;

	return static_cast<T*>(m_elementPointer);
}

template<typename T>
inline void DataItem::operator=(T element)
{
	static_assert(!std::is_pointer_v<T> && !std::is_array_v<T>, "�|�C���^�E�z��͖���");

	if (std::alignment_of_v<T> != m_elementSize)
		throw;

	*static_cast<T*>(m_elementPointer) = element;
	m_cache = false;
}

template<typename T>
inline void DataItem::operator<<(T element)
{
	static_assert(!std::is_pointer_v<T> && !std::is_array_v<T>, "�|�C���^�E�z��͖���");
	m_elementSize = std::alignment_of_v<T>;
	m_elementCount = 0;
	deleteData();
	m_elementPointer = new T(element);
	m_format = getDefaultFormat<T>();
	m_cache = false;
}

template<typename T>
inline void DataItem::shallow(T* elementPointer, size_t elementCount)
{
	m_elementSize = std::alignment_of_v<T>;
	m_elementCount = elementCount;

	deleteData();
	m_elementPointer = elementPointer;
	m_format = getDefaultFormat<T>();
	m_cache = false;
}

template<typename T>
inline void DataItem::deep(const T* elementPointer, size_t elementCount)
{
	m_elementSize = std::alignment_of_v<T>;
	m_elementCount = elementCount;

	deleteData();
	m_elementPointer = new T[elementCount];
	memcpy(m_elementPointer, elementPointer, m_elementSize * elementCount);
	m_format = getDefaultFormat<T>();
	m_cache = false;
}

inline size_t DataItem::getElementSize() const
{
	return m_elementSize;
}

inline size_t DataItem::getElementCount() const
{
	return m_elementCount;
}

inline DataFormat DataItem::getFormat() const
{
	return m_format;
}

inline void DataItem::setFormat(DataFormat format)
{
	if (format == DataFormat::HEX
		|| (format == DataFormat::REAL && (m_elementSize == sizeof(double) || m_elementSize == sizeof(float)))
		|| (format == DataFormat::BOOL && m_elementSize == sizeof(bool))
		|| (format == DataFormat::TEXT && m_elementSize == sizeof(char)))
	{
		m_format = format;
		m_cache = false;
	}
	else
	{
		throw;
	}
}

inline void DataItem::deleteData()
{
	if (m_elementCount == 0)
		delete m_elementPointer;
	else
		delete[] m_elementPointer;
}

template<typename T>
inline DataFormat DataItem::getDefaultFormat()
{
	if (ms_defaultFormat == DefaultDataFormat::AUTO)
	{
		if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
			return DataFormat::REAL;
		else if constexpr (std::is_same_v<T, bool>)
			return DataFormat::BOOL;
		else if constexpr (std::is_same_v<T, char>)
			return DataFormat::TEXT;
	}
	return DataFormat::HEX;
}

