#include <iostream>
#include "CMyStack.h"

char index_signed[] = 
{
  '+',
  '-',
  '*',
  '/',
  '(',
  ')'
};

/*
  >    >    <    <    <    >    >
  >    >    <    <    <    >    >
  >    >    >    >    <    >    >
  >    >    >    >    <    >    >
  <    <    <    <    <    =
  >    >    >    >         >    >
  <    <    <    <    <         =
*/

// >,1  <,2  =,3
char signed_weight[6][6] = 
{
  { 1, 1, 2, 2, 2, 1 },
  { 1, 1, 2, 2, 2, 1 },
  { 1, 1, 1, 1, 2, 1 },
  { 1, 1, 1, 1, 2, 1 },
  { 2, 2, 2, 2, 2, 3 },
  { 1, 1, 1, 1, 0, 1 }
};

//是否是数字符号
bool isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}

//是否是运算符
bool isOperator(char ch)
{
  for (char c : index_signed)
  {
    if (ch == c)
    {
      return true;
    }
  }
  return false;
}

//转换成数字
int chToInt(char ch)
{
  return ch - '0';
}

//取运算符下标
int getOperatorIndex(char ch)
{
  for (int i = 0; i < (sizeof(index_signed) / sizeof(index_signed[0])); i++)
  {
    if (ch == index_signed[i])
    {
      return i;
    }
  }
  return -1;
}

//运算符比较 3相等,1大于,2小于
int campOperator(char left, char right)
{
  return signed_weight[getOperatorIndex(left)][getOperatorIndex(right)];
}

//计算
int Calc(char oper, int lNum, int rNum)
{
  int nRes = 0;
  switch (oper)
  {
    case '+':
    {
      nRes = lNum + rNum;
    }
    break;
    case '-':
    {
      nRes = lNum - rNum;
    }
    break;
    case '*':
    {
      nRes = lNum * rNum;
    }
    break;
    case '/':
    {
      nRes = lNum / rNum;
    }
    break;
  }
  return nRes;
}

int CalcExpression(const char* expression)
{
  //数字栈
  CMyStack<int> number_stack;
  //符号栈
  CMyStack<char> signed_stack;

  //加括号
  std::string newExpre = "(";
  newExpre.append(expression).append(")");

  //临时指针
  char *ptr = const_cast<char *>(newExpre.c_str());

  //遍历
  while (*ptr != '\0')
  {
    //取出当前字符
    char curCh = *ptr;
    char *tmpNum = ptr;
    int nLen = 0;
    int nNumber = 0;

    while (isNumber(*ptr) && *ptr != '\0')
    {
      nLen++;
      ptr++;
    }

    //取数字
    if (nLen > 0)
    {
      //缓冲区
      char tmpBuf[128] = { 0 };
      //拷贝
      memcpy(tmpBuf, tmpNum, nLen);
      //转换
      nNumber = atoi(tmpBuf);
    }

    //是数字
    if (isNumber(curCh))
    {
      number_stack.push(nNumber);
    }
    //是运算符
    else if (isOperator(curCh))
    {
      //右运算符
      char right = curCh;

      //判断栈底是否不为空
      if (!signed_stack.empty())
      {
        //不为空 取top判断
        char left = signed_stack.top();

        //比较
        int cmp = campOperator(left, right);

        //判断比较结果
        switch (cmp)
        {
          case 3://等于
          {
            //等于就抵消
            signed_stack.pop();
          }
          break;
          case 1://大于 大于就计算
          {
            bool bFlag = true;
            do
            {
              //取运算符
              char oper = signed_stack.top();
              signed_stack.pop();
              //取右值
              int rNum = number_stack.top();
              number_stack.pop();
              //取左值
              int lNum = number_stack.top();
              number_stack.pop();
              //运算结果
              int nResNum = Calc(oper, lNum, rNum);
              //压入结果
              number_stack.push(nResNum);
              //符号栈不为空
              if (!signed_stack.empty())
              {
                left = signed_stack.top();
              }
              else
              {
                break;//没有符号弹出
              }

              //判断是否还可以计算
              cmp = campOperator(left, right);
              if (cmp == 3)
              {
                //等于就抵消
                signed_stack.pop();
                bFlag = false;
                break;
              }

            } while (cmp != 2);

            //放入新符号
            if (bFlag != false)
            {
              signed_stack.push(right);
            }
          }
          break;
          case 2://小于
          {
            signed_stack.push(right);
          }
          break;
        }

      }
      else
      {
        signed_stack.push(curCh);
      }
    }

    //非数字
    if (!isNumber(curCh))
    {
      //指针++
      ptr++;
    }
    
  }

  //最后栈里就剩结果
  return number_stack.top();
}

int main()
{
  //表达式
  const char* expression = "(40 * (2 + 8) + (9 + 1)) * 5 * (9 + 1) / (1 + 1) * 5 / 2";

  //计算结果
  int result = CalcExpression(expression);

  //打印
  printf("表达式: %s \r\n结果: %d\r\n", expression, result);
  
  //参考
  printf("%d\r\n", (40 * (2 + 8) + (9 + 1)) * 5 * (9 + 1) / (1 + 1) * 5 / 2);
  
  //暂停
  getchar();
  return 0;
}