#pragma once

/// <summary>
/// <para>DataItem�N���X�̏�Ԓl�𕶎���ɕϊ�����Ƃ��̏����^�C�v</para>
/// <para>DataBox::outputFile()�֐��ȂǂŃt�@�C���o�͂���Ƃ��Ȃǂɓ���</para>
/// <para>HEX=16�i��</para>
/// <para>REAL=����</para>
/// <para>BOOL=�^�U�l</para>
/// <para>TEXT=����</para>
/// </summary>
enum class DataFormat
{
	HEX,
	REAL,
	BOOL,
	TEXT
};

/// <summary>
/// <para>DataItem�N���X��������DataFormat�̃f�t�H���g�l</para>
/// <para>AUTO=�^�ɂ������t�H�[�}�b�g</para>
/// <para>HEX=�ǂ�Ȍ^�ł�HEX</para>
/// </summary>
enum class DefaultDataFormat
{
	AUTO,
	HEX
};