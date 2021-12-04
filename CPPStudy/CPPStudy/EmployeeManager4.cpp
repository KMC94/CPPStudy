#include <iostream>
#include <cstring>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS

// 고용인 클래스,
// 멤버 함수로 1개 이상의 순수 가상 함수를 갖는 클래스를 추상 클래스(Abstract Class)라 한다.
// 따라서 아래의 Employee 클래스는 순수 가상 함수를 멤버 함수로 포함하고 있기 때문에 추상 클래스라 할 수 있다.
class Employee
{
private:
    char name[100];
public:
    Employee(char* name)
    {
        strcpy_s(this->name, name);
    }

    void ShowYourName() const
    {
        cout << "name: " << name << endl;
    }

    // Pure Virtual 함수 -> 함수의 몸체가 없음
    // virtual type func_name() = 0; 이라는 표현을 통해 컴파일러에게 해당 함수가 순수 가상 함수임을 알려 줌
    virtual int GetPay() const = 0;
    virtual void ShowSalaryInfo() const = 0;
};

// 직원 클래스
class PermanentWorker : public Employee
{
private:
    // char name[100]; 상위 클래스에서 이미 선언 되어 있으므로 삭제
    int salary; // 매달 지불해야 하는 급여액
public:
    PermanentWorker(char* name, int money) : Employee(name), salary(money)
    {

    }
    int GetPay() const
    {
        return salary;
    }
    void ShowSalaryInfo() const
    {
        ShowYourName();
        cout << "salary: " << GetPay() << endl << endl;
    }
};

class TemporaryWorker : public Employee
{
private:
    int workTime;
    int payPerHour;
public:
    TemporaryWorker(char* name, int pay) : Employee(name), payPerHour(pay)
    {

    }
    void AddWorkTime(int time)
    {
        workTime += time;
    }
    int GetPay() const
    {
        return workTime * payPerHour;
    }
    void ShowSalaryInfo() const
    {
        ShowYourName();
        cout << "salary: " << GetPay() << endl << endl;
    }

};

// 영업부 클래스
class SalesWorker : public PermanentWorker
{
private:
    int salesResult;
    double bonusRatio;
public:
    SalesWorker(char* name, int money, double ratio) : PermanentWorker(name, money), salesResult(0), bonusRatio(ratio)
    {

    }
    void AddSalesResult(int value)
    {
        salesResult += value;
    }
    int GetPay() const
    {
        return PermanentWorker::GetPay() + (int)(salesResult * bonusRatio);
    }
    void ShowSalaryInfo() const
    {
        ShowYourName();
        cout << "salary: " << GetPay() << endl << endl;
    }
};

namespace RISK_LEVEL
{
    enum
    {
        RISK_A = 30,
        RISK_B = 20,
        RISK_C = 10,
    };
}

class ForeignSalesWorker : public SalesWorker
{
private:
    const int riskLevel;
public:
    ForeignSalesWorker(char* name, int money, double ratio, int risk) :SalesWorker(name, money, ratio), riskLevel(risk)
    {

    }

    int GetRiskPay() const
    {
        return (int)SalesWorker::GetPay() * (riskLevel / 100.0);
    }

    int GetPay() const
    {
        return SalesWorker::GetPay() + GetRiskPay();
    }

    void ShowSalaryInfo() const
    {
        ShowYourName();
        cout << "salary:" << SalesWorker::GetPay() << endl;
        cout << "risk pay: " << GetRiskPay() << endl;
        cout << "sum:" << GetPay() << endl << endl;
    }
};

// 핸들러 클래스 (직원별 정보를 저장)
class EmployeeHandler
{
private:
    // 상위 클래스의 객체 포인터 변수는 하위 클래스의 객체를 가리킬 수 있으므로(하위 클래스의 객체 주소를 저장할 수 있음) 상위 클래스의 이름으로 객체 포인터 배열
    Employee* empList[50];
    int empNum;
public:
    EmployeeHandler() : empNum(0)
    {
        //empty
    }
    void AddEmployee(Employee* emp) // PermanentWorker *emp = new PermenentWorker();
    {
        empList[empNum++] = emp;
    }
    void ShowAllSalaryInfo() const
    {
        for (int i = 0; i < empNum; i++)
        {
            empList[i]->ShowSalaryInfo();
        }
    }
    void ShowTotalSalary() const
    {
        int sum = 0;
        for (int i = 0; i < empNum; i++)
        {
            sum += empList[i]->GetPay();
        }
        cout << "salary sum: " << sum << endl;
    }
    ~EmployeeHandler()
    {
        for (int i = 0; i < empNum; i++)
        {
            delete empList[i];
        }
    }
};

int main()
{
    EmployeeHandler handler;

    // 해외 영업직 등록
    ForeignSalesWorker* fseller1 = new ForeignSalesWorker("Hong", 1000, 0.1, RISK_LEVEL::RISK_A);
    fseller1->AddSalesResult(7000);
    handler.AddEmployee(fseller1);

    ForeignSalesWorker* fseller2 = new ForeignSalesWorker("Yoon", 1000, 0.1, RISK_LEVEL::RISK_B);
    fseller2->AddSalesResult(7000);
    handler.AddEmployee(fseller2);

    ForeignSalesWorker* fseller3 = new ForeignSalesWorker("Lee", 1000, 0.1, RISK_LEVEL::RISK_C);
    fseller3->AddSalesResult(7000);
    handler.AddEmployee(fseller3);

    handler.ShowAllSalaryInfo();
    return 0;
}