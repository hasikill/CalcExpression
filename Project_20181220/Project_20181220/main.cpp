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

//�Ƿ������ַ���
bool isNumber(char ch)
{
  return ch >= '0' && ch <= '9';
}

//�Ƿ��������
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

//ת��������
int chToInt(char ch)
{
  return ch - '0';
}

//ȡ������±�
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

//������Ƚ� 3���,1����,2С��
int campOperator(char left, char right)
{
  return signed_weight[getOperatorIndex(left)][getOperatorIndex(right)];
}

//����
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
  //����ջ
  CMyStack<int> number_stack;
  //����ջ
  CMyStack<char> signed_stack;

  //������
  std::string newExpre = "(";
  newExpre.append(expression).append(")");

  //��ʱָ��
  char *ptr = const_cast<char *>(newExpre.c_str());

  //����
  while (*ptr != '\0')
  {
    //ȡ����ǰ�ַ�
    char curCh = *ptr;
    char *tmpNum = ptr;
    int nLen = 0;
    int nNumber = 0;

    while (isNumber(*ptr) && *ptr != '\0')
    {
      nLen++;
      ptr++;
    }

    //ȡ����
    if (nLen > 0)
    {
      //������
      char tmpBuf[128] = { 0 };
      //����
      memcpy(tmpBuf, tmpNum, nLen);
      //ת��
      nNumber = atoi(tmpBuf);
    }

    //������
    if (isNumber(curCh))
    {
      number_stack.push(nNumber);
    }
    //�������
    else if (isOperator(curCh))
    {
      //�������
      char right = curCh;

      //�ж�ջ���Ƿ�Ϊ��
      if (!signed_stack.empty())
      {
        //��Ϊ�� ȡtop�ж�
        char left = signed_stack.top();

        //�Ƚ�
        int cmp = campOperator(left, right);

        //�жϱȽϽ��
        switch (cmp)
        {
          case 3://����
          {
            //���ھ͵���
            signed_stack.pop();
          }
          break;
          case 1://���� ���ھͼ���
          {
            bool bFlag = true;
            do
            {
              //ȡ�����
              char oper = signed_stack.top();
              signed_stack.pop();
              //ȡ��ֵ
              int rNum = number_stack.top();
              number_stack.pop();
              //ȡ��ֵ
              int lNum = number_stack.top();
              number_stack.pop();
              //������
              int nResNum = Calc(oper, lNum, rNum);
              //ѹ����
              number_stack.push(nResNum);
              //����ջ��Ϊ��
              if (!signed_stack.empty())
              {
                left = signed_stack.top();
              }
              else
              {
                break;//û�з��ŵ���
              }

              //�ж��Ƿ񻹿��Լ���
              cmp = campOperator(left, right);
              if (cmp == 3)
              {
                //���ھ͵���
                signed_stack.pop();
                bFlag = false;
                break;
              }

            } while (cmp != 2);

            //�����·���
            if (bFlag != false)
            {
              signed_stack.push(right);
            }
          }
          break;
          case 2://С��
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

    //������
    if (!isNumber(curCh))
    {
      //ָ��++
      ptr++;
    }
    
  }

  //���ջ���ʣ���
  return number_stack.top();
}

int main()
{
  //���ʽ
  const char* expression = "(40 * (2 + 8) + (9 + 1)) * 5 * (9 + 1) / (1 + 1) * 5 / 2";

  //������
  int result = CalcExpression(expression);

  //��ӡ
  printf("���ʽ: %s \r\n���: %d\r\n", expression, result);
  
  //�ο�
  printf("%d\r\n", (40 * (2 + 8) + (9 + 1)) * 5 * (9 + 1) / (1 + 1) * 5 / 2);
  
  //��ͣ
  getchar();
  return 0;
}