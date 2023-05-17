#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define N 100

void current_time(){
    time_t t;
    struct tm *timeinfo;
    time(&t);
    timeinfo = localtime(&t);
    FILE *fp = fopen("logfile.txt", "a+");
    fprintf(fp,"%s", asctime(timeinfo));
    fclose(fp);
}

struct data
{
    int data[100];
    int top;
};


struct symbol
{
    char symbol[100];
    int top;
};


void InitOperateNum(struct data *StackNum)
{
    StackNum->top = -1;
}


void InitOperateSymbol(struct symbol *StackSymbol)
{
    StackSymbol->top = -1;
}


void Indata(struct data *StackNum, int num)
{
    StackNum->top ++;
    StackNum->data[StackNum->top] = num;
}


void Insymbol(struct symbol *StackSymbol, char ch)
{
    StackSymbol->top ++;
    StackSymbol->symbol[StackSymbol->top] = ch;
}


int Randdata(struct data *StackNum)
{
    return StackNum->data[StackNum->top];
}


char Randsymbol(struct symbol *StackSymbol)
{
    return StackSymbol->symbol[StackSymbol->top];
}


int Putdata(struct data *StackNum)
{
    int x;
    x = StackNum->data[StackNum->top];
    StackNum->top --;
    return x;
}


char Putsymbol(struct symbol *StackSymbol)
{
    char c;
    c = StackSymbol->symbol[StackSymbol->top];
    StackSymbol->top --;
    return c;
}


int judge(char ch) {
    if(ch == '(')
    {
        return 1;
    }
    if(ch == '+' || ch == '-') {
        return 2;
    }
    else if(ch == '*' || ch == '/') {
        return 3;
    }
    else if(ch == ')') {
        return 4;
    }
}


int Math(int v1, int v2, char c)
{
    int sum;
    switch(c) {
        case '+' : {
            sum = v1 + v2;
            break;
        }
        case '-' : {
            sum = v1 - v2;
            break;
        }
        case '*' : {
            sum = v1 * v2;
            break;
        }
        case '/' : {
            sum = v1 / v2;
            break;
        }
    }
    return sum;
}


int calc(){
    printf("输入表达式（符号仅限\"+\" \"-\" \"*\" \"/\" \"(\" \")\"）:\n");
    struct data data;
    struct symbol symbol;
    InitOperateNum(&data);
    InitOperateSymbol(&symbol);
    int i, t, sum, v1, v2;
    char c;
    i = t = sum = 0;
    char v[100] = {0};
    char *str = (char *)malloc(sizeof(char)*200);
    while((c = getchar()) != '\n')
    {
        str[i] = c;
        i ++;
    }
    current_time();
    FILE *fp = fopen("logfile.txt", "a+");
    fprintf(fp, "输入表达式 ");
    for (int j = 0; j < i; ++j) {
        fprintf(fp, "%c",str[j]);
    }
    fprintf(fp, "\n");
    fclose(fp);
    str[i] = '\0';
    for(i = 0; str[i] != '\0'; i ++) {
        if(i == 0 && str[i] == '-') {
            v[t++] = str[i];
        }
        else if(str[i] == '(' && str[i+1] == '-') {
            i ++;
            v[t++] = str[i++];
            while(str[i] >= '0' && str[i] <= '9') {
                v[t] = str[i];
                t ++;
                i ++;
            }
            Indata(&data, atoi(v));
            while(t > 0) {
                v[t] = 0;
                t --;
            }
            if(str[i] != ')') {
                i --;
                Insymbol(&symbol, '(');
            }
        }
        else if(str[i] >= '0' && str[i] <= '9') {
            while(str[i] >= '0' && str[i] <= '9') {
                v[t] = str[i];
                t ++;
                i ++;
            }
            Indata(&data, atoi(v));
            while(t > 0) {
                v[t] = 0;
                t --;
            }
            i --;
        }
        else {
            if(symbol.top == -1)
            {
                Insymbol(&symbol, str[i]);
            }
            else if(judge(str[i]) == 1) {
                Insymbol(&symbol, str[i]);
            }
            else if(judge(str[i]) == 2) {
                if(judge(Randsymbol(&symbol)) == 1) {
                    Insymbol(&symbol, str[i]);
                }
                else if(judge(Randsymbol(&symbol)) == 2) {
                    while(symbol.top >= 0 && data.top >= 1) {
                        v2 = Putdata(&data);
                        v1 = Putdata(&data);
                        sum = Math(v1, v2, Putsymbol(&symbol));
                        Indata(&data, sum);
                    }
                    Insymbol(&symbol, str[i]);
                }
                else if(judge(Randsymbol(&symbol)) == 3) {
                    while(symbol.top >= 0 && data.top >= 1) {
                        v2 = Putdata(&data);
                        v1 = Putdata(&data);
                        sum = Math(v1, v2, Putsymbol(&symbol));
                        Indata(&data, sum);
                    }
                    Insymbol(&symbol, str[i]);
                }

            }
            else if(judge(str[i]) == 3) {
                if(judge(Randsymbol(&symbol)) == 1) {
                    Insymbol(&symbol, str[i]);
                }
                else if(judge(Randsymbol(&symbol)) == 2) {
                    Insymbol(&symbol, str[i]);
                }
                else if(judge(Randsymbol(&symbol)) == 3) {
                    while(symbol.top >= 0 && data.top >= 1) {
                        v2 = Putdata(&data);
                        v1 = Putdata(&data);
                        sum = Math(v1, v2, Putsymbol(&symbol));
                        Indata(&data, sum);
                    }
                    Insymbol(&symbol, str[i]);
                }
            }
            else if(judge(str[i]) == 4) {
                do {
                    v2 = Putdata(&data);
                    v1 = Putdata(&data);
                    sum = Math(v1, v2, Putsymbol(&symbol));
                    Indata(&data, sum);
                }while(judge(Randsymbol(&symbol)) != 1);
                Putsymbol(&symbol);
            }
        }
    }
    free(str);
    while(symbol.top != -1) {
        v2 = Putdata(&data);
        v1 = Putdata(&data);
        sum = Math(v1, v2, Putsymbol(&symbol));
        Indata(&data, sum);
    }
    printf("结果为 %d\n", data.data[0]);
    current_time();
    fp = fopen("logfile.txt", "a+");
    fprintf(fp, "输出结果 %d\n",data.data[0]);
    fclose(fp);

    return 0;
}

double det(double a[N][N], int n)
{
    double res = 0.0;
    double temp[N][N];

    if (n == 1)
    {
        return a[0][0];
    }
    else if (n == 2)
    {
        return a[0][0] * a[1][1] - a[0][1] * a[1][0];
    }
    else
    {
        for (int k = 0; k < n; k++)
        {
            int i, j;
            for (i = 1; i < n; i++)
            {
                int p = 0;
                for (j = 0; j < n; j++)
                {
                    if (j == k)
                        continue;
                    temp[i - 1][p++] = a[i][j];
                }
            }
            res += a[0][k] * det(temp, n - 1) * (k % 2 == 0 ? 1 : -1);
        }
    }
    return res;
}

int matrix()
{
    double a[N][N];
    int n;
    current_time();
    printf("行列数: ");
    scanf("%d", &n);
    FILE *fp = fopen("logfile.txt", "a+");
    fprintf(fp, "输入行列数 %d\n", n);
    printf("按顺序输入每个元素的值：\n");
    fprintf(fp, "输入行列式\n", n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            scanf("%lf", &a[i][j]);
            fprintf(fp, " %lf", a[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    printf("结果为 %lf\n", det(a, n));
    current_time();
    fp = fopen("logfile.txt", "a+");
    fprintf(fp, "输出结果 %lf\n", det(a, n));
    fclose(fp);
    return 0;
}

int main()
{
    FILE *fp;
    fp = fopen("logfile.txt", "a+");
    fprintf(fp, "------------\n");
    fclose(fp);
    start:
    printf("以下是此计算器的功能：\n");
    printf("(1) 计算简单表达式的值\n");
    printf("(2) 计算（反）三角函数\n");
    printf("(3) 计算行列式的值\n");
    printf("(4) 退出\n");
    printf("请输入你要进行的操作序号：\n");
    int operate;
    scanf("%d",&operate);
    getchar();
    if (operate == 1)
    {
        calc();
        printf("若要继续，请输入 1 ，退出请输入 0\n");
        int endoperate;
        scanf("%d",&endoperate);
        if (endoperate == 1)
        {
            goto start;
        }else{
            return 0;
        }

        goto start;
    }else if (operate == 3)
    {
        matrix();
        printf("若要继续，请输入 1 ，退出请输入 0\n");
        int endoperate;
        scanf("%d",&endoperate);
        if (endoperate == 1)
        {
            goto start;
        }else{
            return 0;
        }

        goto start;
    }
    return 0;

}