#include <iostream>
#include "DataBox.h"

int main(void)
{
	// ファイル出力するときにどんな値でも16進数で表示したいときはこれを呼ぶ
	//DataItem::setDefaultDataFormat(DefaultDataFormat::HEX);

	/*
	* このプログラムでは以下の構造のデータを作る
	* 
	* 物
	* ├─食べ物
	* │　├─果物
	* │　│　├─りんご
	* │　│　│　├─色="赤"
	* │　│　│　├─丸い=true
	* │　│　│　└─甘さ=10
	* │　│　└─ぶどう
	* │　│　　　├─甘さ=20
	* │　│　　　└─おいしさ=3.14
	* │　└─野菜
	* │　　　└─きゅうり
	* │　　　　　└─ランク='A'
	* └─乗り物
	* 　　├─車
	* 　　│　├─1="レギュラー"
	* 　　│　├─2="ハイオク"
	* 　　│　├─3="軽油"
	* 　　│　└─ガソリン={1,2,3}
	* 　　└─飛行機={false,true,true,false}
	*/
	DataBox thing;
	{
		DataBox food;
		{
			DataBox fruit;
			{
				DataBox apple;
				apple.add("色", DataItem("赤", sizeof("赤")));
				apple.add("丸い", DataItem(true));
				apple.add("甘さ", DataItem(10));
				fruit.add("りんご", std::move(apple));
			}
			{
				DataBox grape;
				grape.add("甘さ", DataItem(static_cast<uint16_t>(20)));
				grape.add("おいしさ", DataItem(3.14));
				fruit.add("ぶどう", std::move(grape));
			}
			food.add("果物", std::move(fruit));
		}
		{
			DataBox vegetable;
			{
				DataBox cucumber;
				cucumber.add("ランク", DataItem('A'));
				vegetable.add("きゅうり", std::move(cucumber));
			}
			food.add("野菜", std::move(vegetable));
		}
		thing.add("食べ物", std::move(food));
	}
	{
		DataBox vehicle;
		{
			DataBox car;
			car.add("1", DataItem("レギュラー", sizeof("レギュラー")));
			car.add("2", DataItem("ハイオク", sizeof("ハイオク")));
			car.add("3", DataItem("軽油", sizeof("軽油")));
			int gasoline[] = {1, 2, 3};
			car.add("ガソリン", DataItem(gasoline, 3));
			vehicle.add("車", std::move(car));
		}
		{
			bool* plane = new bool[]{false, true, true, false};
			vehicle.add("飛行機", DataItem(plane, 4, false));
		}
		thing.add("乗り物", std::move(vehicle));
	}

	// DataBoxの状態値のファイル入出力が可能
	{
		// 出力
		thing.outputFile("data.txt");

		DataBox thing2;
		// 入力 (ファイル内容の書式が間違っていると例外を出すので注意)
		thing2.inputFile("data.txt");
		thing2.outputFile("data2.txt");
	}

	// operator[]でBoxを辿り、operator()でItemを取得する
	// 最後にItemのoperator()でItemの値を文字列にして出力
	std::cout << "りんごの色=" << thing["食べ物"]["果物"]["りんご"]("色")() << std::endl;

	// Itemは参照できる
	auto& appleRound = thing["食べ物"]["果物"]["りんご"]("丸い");
	std::cout << "りんごは丸いか？=" << appleRound() << std::endl;

	// "丸い"のItemはbool値を入れたので、bool値を代入できる
	appleRound = false;
	std::cout << "りんごは丸いか？=" << appleRound() << std::endl;

	// Boxも参照できる
	auto& grape = thing["食べ物"]["果物"]["ぶどう"];

	// operator()では文字列の取得だが、キャストすると値を取得できる
	// 間違えた形にキャストすると例外を出すので注意
	std::cout << "ぶどうの甘さ=" << grape("甘さ")() << std::endl;
	std::cout << "ぶどうの甘さ=" << static_cast<uint16_t>(grape("甘さ")) << std::endl;

	std::cout << "ぶどうのおいしさ=" << grape("おいしさ")() << std::endl;

	// 暗黙のキャストで値を取得できる
	uint16_t sweetness = grape("甘さ");
	double deliciousness = grape("おいしさ");
	double value = sweetness * deliciousness;
	std::cout << "ぶどうの甘さ×おいしさ=" << value << std::endl;

	// きゅうりランクにはchar型の'A'が入っている
	std::cout << "きゅうりランク=" << thing["食べ物"]["野菜"]["きゅうり"]("ランク")() << std::endl;

	// 型が違う値を代入すると例外を出す
	// thing["食べ物"]["野菜"]["きゅうり"]("ランク") = 500;

	// 型が違う場合は<<演算子で型ごと上書きする
	thing["食べ物"]["野菜"]["きゅうり"]("ランク") << 500;
	std::cout << "きゅうりランク=" << thing["食べ物"]["野菜"]["きゅうり"]("ランク")() << std::endl;

	// 配列や文字列で上書きしたい場合はshallow関数またはdeep関数を呼ぶ
	thing["食べ物"]["野菜"]["きゅうり"]("ランク").deep("きゅうりランクって何だよ！", sizeof("きゅうりランクって何だよ！"));
	std::cout << "きゅうりランク=" << thing["食べ物"]["野菜"]["きゅうり"]("ランク")() << std::endl;

	// 文字列出力の書式を変えることもできる
	thing["食べ物"]["野菜"]["きゅうり"]("ランク").setFormat(DataFormat::HEX);
	std::cout << "きゅうりランク=" << thing["食べ物"]["野菜"]["きゅうり"]("ランク")() << std::endl;

	// 配列を設定していたとしても
	auto& plane = thing["乗り物"]("飛行機");
	std::cout << plane() << std::endl;

	// 違う型で上書きできる
	plane << 1.41421356;
	std::cout << plane() << std::endl;

	// newした配列を設定するときはshallowコピーすれば所有権を渡すことになるのでdelete[]してくれる
	unsigned long long* ary = new unsigned long long[] {12345, 67890, 7777777};
	plane.shallow(ary, 3);
	std::cout << plane() << std::endl;

	return 0;
}
