#pragma once

/// <summary>
/// <para>DataItemクラスの状態値を文字列に変換するときの書式タイプ</para>
/// <para>DataBox::outputFile()関数などでファイル出力するときなどに働く</para>
/// <para>HEX=16進数</para>
/// <para>REAL=実数</para>
/// <para>BOOL=真偽値</para>
/// <para>TEXT=文字</para>
/// </summary>
enum class DataFormat
{
	HEX,
	REAL,
	BOOL,
	TEXT
};

/// <summary>
/// <para>DataItemクラス生成時のDataFormatのデフォルト値</para>
/// <para>AUTO=型にあったフォーマット</para>
/// <para>HEX=どんな型でもHEX</para>
/// </summary>
enum class DefaultDataFormat
{
	AUTO,
	HEX
};