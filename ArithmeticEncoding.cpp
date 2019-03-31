#include <iostream>
#include <map>
#include <fstream>
using namespace std;

/*编码表*/
class ArithmeticEncodingTable
{
  private:
    double low;   //数据区间开始
    double high;  //数据区间结束
    double delta; //数据区间大小
  public:
    ArithmeticEncodingTable();
    ~ArithmeticEncodingTable();
    double get_low();
    double get_high();
    double get_delta();
    void set_low(double low);
    void set_high(double high);
    void set_delta(double delta);
};

/*编码解码类*/
class ArithmeticEncoding
{
  private:
    std::map<char, ArithmeticEncodingTable> CodeTable;

  public:
    ArithmeticEncoding(/* args */);
    ~ArithmeticEncoding();
    void CreateTable();             //生成编码表
    double Encoding(string encode); //编码
    string Decoding(double decode); //解码
};

/*编码所需数据类实现*/
ArithmeticEncodingTable::ArithmeticEncodingTable(/* args */)
{
    low = 0.0;
    high = 1.0;
    delta = 1.0;
}

ArithmeticEncodingTable::~ArithmeticEncodingTable()
{
}

double ArithmeticEncodingTable::get_low()
{
    return low;
}

double ArithmeticEncodingTable::get_high()
{
    return high;
}

double ArithmeticEncodingTable::get_delta()
{
    return delta;
}

void ArithmeticEncodingTable::set_low(double low)
{
    this->low = low;
}

void ArithmeticEncodingTable::set_high(double high)
{
    this->high = high;
}

void ArithmeticEncodingTable::set_delta(double delta)
{
    this->delta = delta;
}

/*编码解码实现*/

ArithmeticEncoding::ArithmeticEncoding(/* args */)
{
}

ArithmeticEncoding::~ArithmeticEncoding()
{
}

void ArithmeticEncoding::CreateTable()
{
    ifstream in("table.txt");
    if (!in)
    {
        cout << "error" << endl;
        return;
    }
    int codenum;
    char code;
    double delta;
    double low = 0.0;
    double high = 0.0;

    cout << "please input number of the char:" << endl;
    in >> codenum;
    for (int i = 0; i < codenum; i++)
    {
        cout << "please input " << i << "th char:";
        in >> code;
        cout << "please input " << i << "th probability:";
        in >> delta;
        ArithmeticEncodingTable table;
        table.set_delta(delta);
        low = high;
        table.set_low(low);
        high = low + delta;
        table.set_high(high);
        CodeTable.insert(std::pair<char, ArithmeticEncodingTable>(code, table));
    }
}
double ArithmeticEncoding::Encoding(string encode)
{

    double low = 0.0;
    double high = 1.0;
    for (string::iterator it = encode.begin(); it != encode.end(); it++)
    {
        double delta = high - low;
        high = low + delta * CodeTable[*it].get_high();
        low = low + delta * CodeTable[*it].get_low();
    }
    return low;
}

string ArithmeticEncoding::Decoding(double value)
{
    double low, high;                   //编码区间
    double prelow = 0.0, prehigh = 1.0; //设置先前区间，方便每次遍历后能回到先前的值
    string symbol = "";                 //输出的解码字符串
    while (true)
    {
        low = prelow;
        high = prehigh;
        for (std::map<char, ArithmeticEncodingTable>::iterator i = CodeTable.begin(); i != CodeTable.end(); ++i)
        {
            double delta = high - low;
            high = low + delta * (i->second).get_high();
            low = low + delta * (i->second).get_low();

            if (low < value && high > value) //判断位置所在区间，在区间内就进行下一次遍历
            {
                prelow = low;
                prehigh = high;
                symbol += (i->first);
                break;
            }
            else if (low == value)
            {
                symbol += (i->first); //最后一个字符输出
                return symbol;
            }
            else //如果不区间，也不是结果，然后回到先前位置
            {
                low = prelow;
                high = prehigh;
            }
        }
    }
}

/*主函数*/
int main()
{

    ArithmeticEncoding AE;
    AE.CreateTable();
    double out = AE.Encoding("CADACDB");
    cout << endl;
    cout << out << endl;
    cout << AE.Decoding(out);
    system("pause");

    return 0;
}