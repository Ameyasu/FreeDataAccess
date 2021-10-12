#pragma once

#include "DataItem.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <mbstring.h>

/// <summary>
/// <para>DataItemクラスを階層構造で所持するクラス</para>
/// <para>DataBoxはDataBoxとDataItemを所持する</para>
/// <para>ファイル入出力を使用する場合のpathの禁則文字</para>
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
	/// <para>DataBoxにアクセスする</para>
	/// <para>パスが存在しない場合例外</para>
	/// </summary>
	const DataBox& operator[](const char* path) const;

	/// <summary>
	/// <para>DataBoxにアクセスする</para>
	/// <para>パスが存在しない場合例外</para>
	/// </summary>
	DataBox& operator[](const char* path);

	/// <summary>
	/// <para>DataItemにアクセスする</para>
	/// <para>パスが存在しない場合例外</para>
	/// </summary>
	const DataItem& operator()(const char* path) const;

	/// <summary>
	/// <para>DataItemにアクセスする</para>
	/// <para>パスが存在しない場合例外</para>
	/// </summary>
	DataItem& operator()(const char* path);

	/// <returns>true=DataBoxのパスが存在する</returns>
	bool box(const char* path) const;

	/// <returns>true=DataItemのパスが存在する</returns>
	bool item(const char* path) const;

	/// <summary>
	/// <para>DataBoxを追加する</para>
	/// <para>パスが存在する場合上書き</para>
	/// </summary>
	void add(const char* path, DataBox&& box);

	/// <summary>
	/// <para>DataItemを追加する</para>
	/// <para>パスが存在する場合上書き</para>
	/// </summary>
	void add(const char* path, DataItem&& item);

	/// <summary>
	/// <para>DataBoxの状態値をファイルから入力する</para>
	/// <para>成功した場合、既存の状態値は全て消える</para>
	/// <para>ファイルの書式が間違っていると例外</para>
	/// </summary>
	/// <param name="path">入力ファイルパス</param>
	/// <returns>true=成功, false=失敗</returns>
	bool inputFile(const char* path);

	/// <summary>
	/// <para>DataBoxの状態値をファイルへ出力する</para>
	/// </summary>
	/// <param name="path">出力ファイルパス</param>
	/// <returns>true=成功, false=失敗</returns>
	bool outputFile(const char* path) const;

	/// <summary>
	/// <para>中身を全て削除する</para>
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
	/// <para>全角か半角かを見分けながら指定した半角文字のインデックスを検索する</para>
	/// <para>検索半角文字を見つける前に文字列の最後に到達したら例外</para>
	/// </summary>
	/// <param name="start">検索開始インデックス</param>
	/// <param name="sbc">検索半角文字</param>
	/// <returns>最初に見つけた検索半角文字のインデックス</returns>
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

	// formatText全体をループする
	int i = 0;
	while (i < size)
	{
		// 全角文字は読み飛ばす
		if (_mbclen(text + i) == 2)
		{
			i += 2;
			continue;
		}

		// DataItemのタグを見つけたとき
		if (text[i] == '(')
		{
			// 対応するタグ閉じのインデックスを取得
			int j = foundSBC(i + 1, ')');

			// 改行インデックスを取得
			int k = foundSBC(j + 1, '\n');

			// 今の状況
			// i            j     k
			// (DataItemName)Value

			// add("DataItemName", DataItem("Value");
			add(formatText.substr(i + 1, j - i - 1).c_str(), DataItem(formatText.substr(j + 1, k - j - 1).c_str()));

			// 次に++iされるのでここで改行を指しておくと丁度いい
			i = k;
		}

		// DataBoxのタグを見つけたとき
		else if (text[i] == '[')
		{
			// 対応するタグ閉じのインデックスを取得
			int j = foundSBC(i + 1, ']');

			// 今の状況
			// i           j
			// [DataBoxName]

			// std::string name = "DataBoxName";
			std::string name = formatText.substr(i + 1, j - i - 1);

			// [DataBoxName]に対応する[/DataBoxName]を検索
			// [DataBoxName][DataBoxName][/DataBoxName][/DataBoxName]のような入れ子にも対応
			int c = 1;
			int k = j + 1;
			int l = 0;
			while (true)
			{
				// [/DataBoxName]を見つける前にformatTextが終了したら例外
				if (k >= size)
					throw;

				// 全角は読み飛ばす
				if (_mbclen(text + k) == 2)
				{
					k += 2;
					continue;
				}

				// DataBoxのタグを見つけたとき
				if (text[k] == '[')
				{
					// 次の文字がなければ例外
					if (k + 1 >= size)
						throw;

					// 次の文字が'/'だった場合
					if (_mbclen(text + k + 1) == 1 && text[k + 1] == '/')
					{
						// 対応するタグ閉じのインデックスを取得
						l = foundSBC(k + 2, ']');

						// 今の状況
						// k            l
						// [/DataBoxName]

						// std::string name2 = "DataBoxName";
						std::string name2 = formatText.substr(k + 2, l - k - 2);

						if (name == name2)
						{
							// 名前が一致したら入れ子カウントを減らす
							--c;

							// ゼロになったら[DataBoxName]に対応する[/DataBoxName]を見つけたということ
							// 検索終了
							if (c == 0)
								break;
						}
					}
					else
					{
						// 対応するタグ閉じのインデックスを取得
						l = foundSBC(k + 1, ']');

						// 今の状況
						// k           l
						// [DataBoxName]

						// std::string name2 = "DataBoxName";
						std::string name2 = formatText.substr(k + 1, l - k - 1);

						// 名前が一致したら入れ子カウントを増やす
						if (name == name2)
							++c;
					}

					// 次に++kされるのでここで']'を指しておくと丁度いい
					k = l;
				}
				++k;
			}

			// 検索に失敗したら例外
			if (c != 0)
				throw;

			// 今の状況
			// i           j
			// [DataBoxName]
			// 
			//   (中略)
			// 
			// k            l
			// [/DataBoxName]

			// DataBoxNameという名前のDataBoxの中身は再帰的に設定すればOK
			// box.input("(中略)");
			DataBox box;
			box.input(formatText.substr(j + 1, k - j - 1));
			add(name.c_str(), std::move(box));

			// 次に++iされるのでここで']'を指しておくと丁度いい
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

