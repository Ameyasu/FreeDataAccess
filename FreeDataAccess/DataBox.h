#pragma once

#include "DataItem.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <mbstring.h>

/// <summary>
/// <para>DataItem�N���X���K�w�\���ŏ�������N���X</para>
/// <para>DataBox��DataBox��DataItem����������</para>
/// <para>�t�@�C�����o�͂��g�p����ꍇ��path�֑̋�����</para>
/// <para>[ ] ( ){ } / ' " $</para>
/// </summary>
class DataBox
{
public:
	DataBox();
	~DataBox();

	DataBox(const DataBox&) = delete;
	DataBox& operator=(const DataBox&) = delete;

	DataBox(DataBox&&) noexcept = default;
	DataBox& operator=(DataBox&&) noexcept = default;

public:
	/// <summary>
	/// <para>DataBox�ɃA�N�Z�X����</para>
	/// <para>�p�X�����݂��Ȃ��ꍇ��O</para>
	/// </summary>
	const DataBox& operator[](const char* path) const;

	/// <summary>
	/// <para>DataBox�ɃA�N�Z�X����</para>
	/// <para>�p�X�����݂��Ȃ��ꍇ��O</para>
	/// </summary>
	DataBox& operator[](const char* path);

	/// <summary>
	/// <para>DataItem�ɃA�N�Z�X����</para>
	/// <para>�p�X�����݂��Ȃ��ꍇ��O</para>
	/// </summary>
	const DataItem& operator()(const char* path) const;

	/// <summary>
	/// <para>DataItem�ɃA�N�Z�X����</para>
	/// <para>�p�X�����݂��Ȃ��ꍇ��O</para>
	/// </summary>
	DataItem& operator()(const char* path);

	/// <returns>true=DataBox�̃p�X�����݂���</returns>
	bool box(const char* path) const;

	/// <returns>true=DataItem�̃p�X�����݂���</returns>
	bool item(const char* path) const;

	/// <summary>
	/// <para>DataBox��ǉ�����</para>
	/// <para>�p�X�����݂���ꍇ�㏑��</para>
	/// </summary>
	void add(const char* path, DataBox&& box);

	/// <summary>
	/// <para>DataItem��ǉ�����</para>
	/// <para>�p�X�����݂���ꍇ�㏑��</para>
	/// </summary>
	void add(const char* path, DataItem&& item);

	/// <summary>
	/// <para>DataBox�̏�Ԓl���t�@�C��������͂���</para>
	/// <para>���������ꍇ�A�����̏�Ԓl�͑S�ď�����</para>
	/// <para>�t�@�C���̏������Ԉ���Ă���Ɨ�O</para>
	/// </summary>
	/// <param name="path">���̓t�@�C���p�X</param>
	/// <returns>true=����, false=���s</returns>
	bool inputFile(const char* path);

	/// <summary>
	/// <para>DataBox�̏�Ԓl���t�@�C���֏o�͂���</para>
	/// </summary>
	/// <param name="path">�o�̓t�@�C���p�X</param>
	/// <returns>true=����, false=���s</returns>
	bool outputFile(const char* path) const;

	/// <summary>
	/// <para>���g��S�č폜����</para>
	/// </summary>
	void clear();

private:
	void input(const std::string& formatText);
	std::string output(const std::string& indent = std::string()) const;

private:
	std::map<std::string, DataBox, std::less<>> m_box;
	std::map<std::string, DataItem, std::less<>> m_item;
};




inline DataBox::DataBox()
	: m_box()
	, m_item()
{
}

inline DataBox::~DataBox()
{
}

inline const DataBox& DataBox::operator[](const char* path) const
{
	return m_box.at(path);
}

inline DataBox& DataBox::operator[](const char* path)
{
	return m_box.at(path);
}

inline const DataItem& DataBox::operator()(const char* path) const
{
	return m_item.at(path);
}

inline DataItem& DataBox::operator()(const char* path)
{
	return m_item.at(path);
}

inline bool DataBox::box(const char* path) const
{
	return m_box.count(path) == 1;
}

inline bool DataBox::item(const char* path) const
{
	return m_item.count(path) == 1;
}

inline void DataBox::add(const char* path, DataBox&& box)
{
	m_box.emplace(path, std::move(box));
}

inline void DataBox::add(const char* path, DataItem&& item)
{
	m_item.emplace(path, std::move(item));
}

inline bool DataBox::inputFile(const char* path)
{
	std::ifstream o(path, std::ios::in);
	if (!o)
		return false;

	clear();

	std::string s((std::istreambuf_iterator<char>(o)), std::istreambuf_iterator<char>());
	input(s);

	return true;
}

inline bool DataBox::outputFile(const char* path) const
{
	std::ofstream o(path, std::ios::out);
	if (!o)
		return false;

	std::string s = output();
	o.write(s.c_str(), s.size());

	return true;
}

inline void DataBox::clear()
{
	m_box.clear();
	m_item.clear();
}

inline void DataBox::input(const std::string& formatText)
{
	const unsigned char* text = reinterpret_cast<const unsigned char*>(formatText.c_str());
	int size = formatText.size();

	/// <summary>
	/// <para>�S�p�����p�����������Ȃ���w�肵�����p�����̃C���f�b�N�X����������</para>
	/// <para>�������p������������O�ɕ�����̍Ō�ɓ��B�������O</para>
	/// </summary>
	/// <param name="start">�����J�n�C���f�b�N�X</param>
	/// <param name="sbc">�������p����</param>
	/// <returns>�ŏ��Ɍ������������p�����̃C���f�b�N�X</returns>
	auto foundSBC = [text, size](int start, char sbc)
	{
		while (true)
		{
			if (start >= size)
				throw;
			if (_mbclen(text + start) == 2)
			{
				start += 2;
				continue;
			}
			if (text[start] == sbc)
				break;
			++start;
		}
		return start;
	};

	// formatText�S�̂����[�v����
	int i = 0;
	while (i < size)
	{
		// �S�p�����͓ǂݔ�΂�
		if (_mbclen(text + i) == 2)
		{
			i += 2;
			continue;
		}

		// DataItem�̃^�O���������Ƃ�
		if (text[i] == '(')
		{
			// �Ή�����^�O���̃C���f�b�N�X���擾
			int j = foundSBC(i + 1, ')');

			// ���s�C���f�b�N�X���擾
			int k = foundSBC(j + 1, '\n');

			// ���̏�
			// i            j     k
			// (DataItemName)Value

			// add("DataItemName", DataItem("Value");
			add(formatText.substr(i + 1, j - i - 1).c_str(), DataItem(formatText.substr(j + 1, k - j - 1).c_str()));

			// ����++i�����̂ł����ŉ��s���w���Ă����ƒ��x����
			i = k;
		}

		// DataBox�̃^�O���������Ƃ�
		else if (text[i] == '[')
		{
			// �Ή�����^�O���̃C���f�b�N�X���擾
			int j = foundSBC(i + 1, ']');

			// ���̏�
			// i           j
			// [DataBoxName]

			// std::string name = "DataBoxName";
			std::string name = formatText.substr(i + 1, j - i - 1);

			// [DataBoxName]�ɑΉ�����[/DataBoxName]������
			// [DataBoxName][DataBoxName][/DataBoxName][/DataBoxName]�̂悤�ȓ���q�ɂ��Ή�
			int c = 1;
			int k = j + 1;
			int l = 0;
			while (true)
			{
				// [/DataBoxName]��������O��formatText���I���������O
				if (k >= size)
					throw;

				// �S�p�͓ǂݔ�΂�
				if (_mbclen(text + k) == 2)
				{
					k += 2;
					continue;
				}

				// DataBox�̃^�O���������Ƃ�
				if (text[k] == '[')
				{
					// ���̕������Ȃ���Η�O
					if (k + 1 >= size)
						throw;

					// ���̕�����'/'�������ꍇ
					if (_mbclen(text + k + 1) == 1 && text[k + 1] == '/')
					{
						// �Ή�����^�O���̃C���f�b�N�X���擾
						l = foundSBC(k + 2, ']');

						// ���̏�
						// k            l
						// [/DataBoxName]

						// std::string name2 = "DataBoxName";
						std::string name2 = formatText.substr(k + 2, l - k - 2);

						if (name == name2)
						{
							// ���O����v���������q�J�E���g�����炷
							--c;

							// �[���ɂȂ�����[DataBoxName]�ɑΉ�����[/DataBoxName]���������Ƃ�������
							// �����I��
							if (c == 0)
								break;
						}
					}
					else
					{
						// �Ή�����^�O���̃C���f�b�N�X���擾
						l = foundSBC(k + 1, ']');

						// ���̏�
						// k           l
						// [DataBoxName]

						// std::string name2 = "DataBoxName";
						std::string name2 = formatText.substr(k + 1, l - k - 1);

						// ���O����v���������q�J�E���g�𑝂₷
						if (name == name2)
							++c;
					}

					// ����++k�����̂ł�����']'���w���Ă����ƒ��x����
					k = l;
				}
				++k;
			}

			// �����Ɏ��s�������O
			if (c != 0)
				throw;

			// ���̏�
			// i           j
			// [DataBoxName]
			// 
			//   (����)
			// 
			// k            l
			// [/DataBoxName]

			// DataBoxName�Ƃ������O��DataBox�̒��g�͍ċA�I�ɐݒ肷���OK
			// box.input("(����)");
			DataBox box;
			box.input(formatText.substr(j + 1, k - j - 1));
			add(name.c_str(), std::move(box));

			// ����++i�����̂ł�����']'���w���Ă����ƒ��x����
			i = l;
		}

		++i;
	}
}

inline std::string DataBox::output(const std::string& indent) const
{
	std::stringstream s;

	for (auto& i : m_item)
		s << indent << '(' << i.first << ')' << i.second() << '\n';

	std::string n = indent + "  ";

	for (auto& i : m_box)
	{
		s << indent << '[' << i.first << "]\n";
		s << i.second.output(n);
		s << indent << "[/" << i.first << "]\n";
	}

	return s.str();
}

