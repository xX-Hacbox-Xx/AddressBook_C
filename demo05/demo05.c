#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct friends_list /*手机通讯录结构定义*/
{
    char name[10];      /* 姓名 */
    int age;            /* 年龄 */
    char telephone[13]; /* 联系电话 */
};
int Count = 0;
FILE *fp;
void new_friend(struct friends_list friends[]);
void search_friend(struct friends_list friends[], char *name);
void delete_friend(struct friends_list friends[], char *name);
void read_friend_data(struct friends_list friends[]);
void write_friend_data(struct friends_list friends[]);
void read_count_data(struct friends_list friends[]);
void write_count_data(struct friends_list friends[]);
int getNextLinePos(FILE *p);
int main()
{
    int choice, i;
    char name[10];
    struct friends_list friends[50]; /* 包含50个人的通讯录 */
    read_friend_data(friends);
    do
    {
        printf("手机通讯录功能选项：1:新建 2:查询 3:删除 0:保存并退出\n");
        printf("请选择功能：");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            new_friend(friends);
            break;
        case 2:
            printf("请输入要查找的联系人名:");
            scanf("%s", name);
            search_friend(friends, name);
            break;
        case 3:
            printf("请输入要删除的联系人名:");
            scanf("%s", name);
            delete_friend(friends, name);
            break;
        case 0:
            write_friend_data(friends);
            break;
        }
    } while (choice != 0);

    printf("谢谢使用通讯录功能!\n");
    return 0;
}

void delete_friend(struct friends_list friends[], char *name)
{
    int i, flag = 0;
    if (Count == 0)
    {
        printf("通讯录是空的!\n");
        return;
    }
    for (i = 0; i < Count; i++)
        if (strcmp(name, friends[i].name) == 0)
        { /* 找到联系人*/
            flag = 1;
            break;
        }
    for (; i < Count; i++)
        friends[i] = friends[i + 1];
}
void new_friend(struct friends_list friends[])
{
    read_count_data(friends);
    printf("通讯录人数：%d\n", Count);
    struct friends_list f;
    if (Count == 50)
    {
        printf("通讯录已满!\n");
        return;
    }
    printf("请输入新联系人的姓名:");
    scanf("%s", f.name);
    printf("请输入新联系人的年龄:");
    scanf("%d", &f.age);
    printf("请输入新联系人的联系电话:");
    scanf("%s", f.telephone);
    friends[Count] = f;
    Count++;
    write_count_data(friends);
}
void search_friend(struct friends_list friends[], char *name)
{
    int i, flag = 0;
    read_count_data(friends);
    printf("通讯录人数：%d\n", Count);
    if (Count == 0)
    {
        printf("通讯录是空的!\n");
        return;
    }
    for (i = 0; i < Count; i++)
        if (strcmp(name, friends[i].name) == 0)
        { /* 找到联系人*/
            flag = 1;
            break;
        }
    if (flag)
    {
        printf("姓名: %s\t", friends[i].name);
        printf("年龄: %d\t", friends[i].age);
        printf("电话: %s\n", friends[i].telephone);
    }
    else
        printf("无此联系人!");
}
void read_friend_data(struct friends_list friends[])
{
    read_count_data(friends);
    if (Count == 0)
        return;
    int i, j;
    char s[3] = {};
    if ((fp = fopen("data.txt ", "rb")) == NULL)        //这里的文件目录需要修改
    {
        printf("无法打开通讯录文件！\n");
        exit(0);
    }
    fseek(fp, getNextLinePos(fp), SEEK_SET); // 指向下一行
    for (j = 0; j < 50; j++)
    { /* 读取通讯录 */
        fread(&friends[j], sizeof(struct friends_list), 1, fp);
    }
    if (fclose(fp))
    { /* 关闭文件 */
        printf("Can not close the file!\n");
        exit(0);
    }
}
void write_friend_data(struct friends_list friends[])
{
    int i = 0;
    if ((fp = fopen("data.txt", "r+b")) == NULL)
    {
        printf("无法打开通讯录文件！\n");
        exit(0);
    }
    fseek(fp, 0L, SEEK_SET);
    fseek(fp, getNextLinePos(fp), SEEK_SET); // 指向下一行
    for (i = 0; i < 50; i++)
    { /* 写入通讯录 */
        if (fwrite(&friends[i], sizeof(struct friends_list), 1, fp) != 1)
            printf("file write error\n");
    }
    if (fclose(fp))
    { /* 关闭文件 */
        printf("Can not close the file!\n");
        exit(0);
    }
}
void read_count_data(struct friends_list friends[])
{
    char s[3] = {};
    int n, i;
    if ((fp = fopen("data.txt", "r")) == NULL)
    {
        printf("无法打开通讯录文件！\n");
        exit(0);
    }
    fseek(fp, 0L, SEEK_SET);
    fscanf(fp, "%s\n", &s);
    for (i = 0, n = 0; s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = 10 * n + (s[i] - '0');
    }
    Count = n;
    fseek(fp, 0L, SEEK_SET);
    if (fclose(fp))
    { /* 关闭文件 */
        printf("Can not close the file!\n");
        exit(0);
    }
}
void write_count_data(struct friends_list friends[])
{
    char s[2] = {};
    int n, i;
    if ((fp = fopen("data.txt", "r+")) == NULL)
    {
        printf("无法打开通讯录文件！\n");
        exit(0);
    }
    fseek(fp, 0L, SEEK_SET);
    itoa(Count, s, 10);
    fprintf(fp, "%s\n", s);
    fseek(fp, 0L, SEEK_SET);
    if (fclose(fp))
    { /* 关闭文件 */
        printf("Can not close the file!\n");
        exit(0);
    }
}
int getNextLinePos(FILE *p)
{
    int ch;
    int curpos = 0;
    ch = getc(p);
    curpos = ftell(p);
    while (ch != EOF)
    {
        putchar(ch);
        ch = getc(p);
        if (ch == '\n')
        {
            break;
        }
    }
    return (ftell(p) - curpos + 1);
}
