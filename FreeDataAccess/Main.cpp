#include <iostream>
#include "DataBox.h"

int main(void)
{
	// �t�@�C���o�͂���Ƃ��ɂǂ�Ȓl�ł�16�i���ŕ\���������Ƃ��͂�����Ă�
	//DataItem::setDefaultDataFormat(DefaultDataFormat::HEX);

	/*
	* ���̃v���O�����ł͈ȉ��̍\���̃f�[�^�����
	* 
	* ��
	* �����H�ו�
	* ���@�����ʕ�
	* ���@���@�������
	* ���@���@���@�����F="��"
	* ���@���@���@�����ۂ�=true
	* ���@���@���@�����Â�=10
	* ���@���@�����Ԃǂ�
	* ���@���@�@�@�����Â�=20
	* ���@���@�@�@������������=3.14
	* ���@�������
	* ���@�@�@�������イ��
	* ���@�@�@�@�@���������N='A'
	* ������蕨
	* �@�@������
	* �@�@���@����1="���M�����["
	* �@�@���@����2="�n�C�I�N"
	* �@�@���@����3="�y��"
	* �@�@���@�����K�\����={1,2,3}
	* �@�@������s�@={false,true,true,false}
	*/
	DataBox thing;
	{
		DataBox food;
		{
			DataBox fruit;
			{
				DataBox apple;
				apple.add("�F", DataItem("��", sizeof("��")));
				apple.add("�ۂ�", DataItem(true));
				apple.add("�Â�", DataItem(10));
				fruit.add("���", std::move(apple));
			}
			{
				DataBox grape;
				grape.add("�Â�", DataItem(static_cast<uint16_t>(20)));
				grape.add("��������", DataItem(3.14));
				fruit.add("�Ԃǂ�", std::move(grape));
			}
			food.add("�ʕ�", std::move(fruit));
		}
		{
			DataBox vegetable;
			{
				DataBox cucumber;
				cucumber.add("�����N", DataItem('A'));
				vegetable.add("���イ��", std::move(cucumber));
			}
			food.add("���", std::move(vegetable));
		}
		thing.add("�H�ו�", std::move(food));
	}
	{
		DataBox vehicle;
		{
			DataBox car;
			car.add("1", DataItem("���M�����[", sizeof("���M�����[")));
			car.add("2", DataItem("�n�C�I�N", sizeof("�n�C�I�N")));
			car.add("3", DataItem("�y��", sizeof("�y��")));
			int gasoline[] = {1, 2, 3};
			car.add("�K�\����", DataItem(gasoline, 3));
			vehicle.add("��", std::move(car));
		}
		{
			bool* plane = new bool[]{false, true, true, false};
			vehicle.add("��s�@", DataItem(plane, 4, false));
		}
		thing.add("��蕨", std::move(vehicle));
	}

	// DataBox�̏�Ԓl�̃t�@�C�����o�͂��\
	{
		// �o��
		thing.outputFile("data.txt");

		DataBox thing2;
		// ���� (�t�@�C�����e�̏������Ԉ���Ă���Ɨ�O���o���̂Œ���)
		thing2.inputFile("data.txt");
		thing2.outputFile("data2.txt");
	}

	// operator[]��Box��H��Aoperator()��Item���擾����
	// �Ō��Item��operator()��Item�̒l�𕶎���ɂ��ďo��
	std::cout << "��񂲂̐F=" << thing["�H�ו�"]["�ʕ�"]["���"]("�F")() << std::endl;

	// Item�͎Q�Ƃł���
	auto& appleRound = thing["�H�ו�"]["�ʕ�"]["���"]("�ۂ�");
	std::cout << "��񂲂͊ۂ����H=" << appleRound() << std::endl;

	// "�ۂ�"��Item��bool�l����ꂽ�̂ŁAbool�l�����ł���
	appleRound = false;
	std::cout << "��񂲂͊ۂ����H=" << appleRound() << std::endl;

	// Box���Q�Ƃł���
	auto& grape = thing["�H�ו�"]["�ʕ�"]["�Ԃǂ�"];

	// operator()�ł͕�����̎擾�����A�L���X�g����ƒl���擾�ł���
	// �ԈႦ���`�ɃL���X�g����Ɨ�O���o���̂Œ���
	std::cout << "�Ԃǂ��̊Â�=" << grape("�Â�")() << std::endl;
	std::cout << "�Ԃǂ��̊Â�=" << static_cast<uint16_t>(grape("�Â�")) << std::endl;

	std::cout << "�Ԃǂ��̂�������=" << grape("��������")() << std::endl;

	// �Öق̃L���X�g�Œl���擾�ł���
	uint16_t sweetness = grape("�Â�");
	double deliciousness = grape("��������");
	double value = sweetness * deliciousness;
	std::cout << "�Ԃǂ��̊Â��~��������=" << value << std::endl;

	// ���イ�胉���N�ɂ�char�^��'A'�������Ă���
	std::cout << "���イ�胉���N=" << thing["�H�ו�"]["���"]["���イ��"]("�����N")() << std::endl;

	// �^���Ⴄ�l��������Ɨ�O���o��
	// thing["�H�ו�"]["���"]["���イ��"]("�����N") = 500;

	// �^���Ⴄ�ꍇ��<<���Z�q�Ō^���Ə㏑������
	thing["�H�ו�"]["���"]["���イ��"]("�����N") << 500;
	std::cout << "���イ�胉���N=" << thing["�H�ו�"]["���"]["���イ��"]("�����N")() << std::endl;

	// �z��╶����ŏ㏑���������ꍇ��shallow�֐��܂���deep�֐����Ă�
	thing["�H�ו�"]["���"]["���イ��"]("�����N").deep("���イ�胉���N���ĉ�����I", sizeof("���イ�胉���N���ĉ�����I"));
	std::cout << "���イ�胉���N=" << thing["�H�ו�"]["���"]["���イ��"]("�����N")() << std::endl;

	// ������o�͂̏�����ς��邱�Ƃ��ł���
	thing["�H�ו�"]["���"]["���イ��"]("�����N").setFormat(DataFormat::HEX);
	std::cout << "���イ�胉���N=" << thing["�H�ו�"]["���"]["���イ��"]("�����N")() << std::endl;

	// �z���ݒ肵�Ă����Ƃ��Ă�
	auto& plane = thing["��蕨"]("��s�@");
	std::cout << plane() << std::endl;

	// �Ⴄ�^�ŏ㏑���ł���
	plane << 1.41421356;
	std::cout << plane() << std::endl;

	// new�����z���ݒ肷��Ƃ���shallow�R�s�[����Ώ��L����n�����ƂɂȂ�̂�delete[]���Ă����
	unsigned long long* ary = new unsigned long long[] {12345, 67890, 7777777};
	plane.shallow(ary, 3);
	std::cout << plane() << std::endl;

	return 0;
}
