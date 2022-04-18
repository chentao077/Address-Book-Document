#include"contact.h"
void CheckContact(struct contact* ps);
void InitContact(struct contact* ps)
{
	ps->data = (struct People_Init*)malloc(3 * sizeof(struct People_Init));
	ps->capacity = 3;
	ps->size = 0;
	//加载之前保存的数据
	struct People_Init tmp = { 0 };
	FILE* pfRead = fopen("contact.bat", "rb");
	//如果存储文件不存在，生成文件
	if (pfRead == NULL)
	{
		FILE* pfNew = fopen("contact.bat", "wb+");
		fclose(pfNew);
		pfNew = NULL;
	}
//again方法
//again:
//	FILE* pfRead = fopen("contact.bat", "rb");
//	struct People_Init tmp = { 0 };
//	if (pfRead == NULL)
//	{
//		//printf("%s", strerror(errno));
//		FILE* pfRead = fopen("contact.bat", "wb+");
//		fclose(pfRead);
//		pfRead = NULL;
//		goto again;
//	}
	while (fread(&tmp, sizeof(struct People_Init), 1, pfRead))//fread返回值是最后一次读取的实际数量
	{
		//检查容量
		CheckContact(ps);
		//赋值
		ps->data[ps->size] = tmp;
		ps->size++;
	}
	fclose(pfRead);
	pfRead = NULL;
}
void CheckContact(struct contact* ps)
{
	if (ps->capacity == ps->size)
	{
		struct People_Init* ptr = realloc(ps->data, (ps->capacity + 2) * sizeof(struct People_Init));
		if (ptr != NULL)
		{
			ps->capacity += 2;
			ps->data = ptr;
			printf("增容成功\n");
		}
	}
}

void AddContact(struct contact* ps)
{
	//检查通讯录空间
	//1.不够开辟空间
	//2.够了不管
	CheckContact(ps);
	printf("请输入姓名：");
	scanf("%s", ps->data[ps->size].name);
	printf("请输入年龄：");
	scanf("%d", &(ps->data[ps->size].age));
	printf("请输入性别：");
	scanf("%s", ps->data[ps->size].sex);
	printf("请输入电话：");
	scanf("%s", ps->data[ps->size].tele);
	printf("请输入地址：");
	scanf("%s", ps->data[ps->size].addr);
	(ps->size)++;
	printf("输入成功\n");
}
void PrintContact(const struct contact* ps)
{
	if (ps->size == 0)
	{
		printf("通讯录为空\n");
	}
	else
	{
		int i = 0;
		//打印标题
		printf("%-10s\t%-4s\t%-5s\t%-12s\t%-30s\n", "姓名", "年龄", "性别", "电话", "地址");
		for ( i = 0; i < ps->size; i++)
		{
			printf("%-10s\t%-4d\t%-5s\t%-12s\t%-30s\n", 
				ps->data[i].name,
				ps->data[i].age,
				ps->data[i].sex,
				ps->data[i].tele,
				ps->data[i].addr);
		}
	}
}
//封装查找功能，找到返回下标，找不到返回-1
//static修饰，这个函数只在test.c源码中使用
static int Find_Name(const struct contact* ps, char name[Name_max])
{
	int i = 0;
	for (i = 0; i < ps->size; i++)
	{
		if (0 == strcmp(ps->data[i].name, name))
		{
			return i;
		}
	}
	return -1;
}
void DelContact(struct contact* ps) 
{
	//1.查找
	char name[Name_max];	
	printf("请输入要删除的名字：");
	scanf("%s", name);
	int ret=Find_Name(ps, name);
	
	//2.删除
	if (ret == -1)
	{
		printf("要删除的人不存在\n");
	}
	else
	{
		int j = 0;
		for ( j = ret; j < ps->size-1; j++)
		{
			ps->data[j] = ps->data[j + 1];
		}
		ps->size--;
		printf("删除成功\n");
	}
}
void SearchContact(const struct contact* ps)
{
	char name[Name_max];
	printf("请输入要查找的名字：");
	scanf("%s", name);
	int ret = Find_Name(ps, name);
	if (ret == -1)
	{
		printf("要查找的人不存在\n");
	}
	else
	{
		printf("%-10s\t%-4s\t%-5s\t%-12s\t%-30s\n", "姓名", "年龄", "性别", "电话", "地址");
		printf("%-10s\t%-4d\t%-5s\t%-12s\t%-30s\n",
				ps->data[ret].name,
				ps->data[ret].age,
				ps->data[ret].sex,
				ps->data[ret].tele,
				ps->data[ret].addr);	
	}
}
void ModifyContact(const struct contact* ps)
{
	char name[Name_max];
	printf("请输入要修改的名字：");
	scanf("%s", name);
	int ret = Find_Name(ps, name);
	if (ret == -1)
	{
		printf("要修改的人不存在\n");
	}
	else
	{
		printf("请输入姓名：");
		scanf("%s", ps->data[ret].name);
		printf("请输入年龄：");
		scanf("%d", &(ps->data[ret].age));
		printf("请输入性别：");
		scanf("%s", ps->data[ret].sex);
		printf("请输入电话：");
		scanf("%s", ps->data[ret].tele);
		printf("请输入地址：");
		scanf("%s", ps->data[ret].addr);
		
		printf("修改成功\n");
	}
}
int Cmp_Name(const void* p1, const void* p2)
{
	return strcmp(((struct People_Init*)p1)->name, ((struct People_Init*)p2)->name);	
}
void SortContact(struct contact* ps)
{
	qsort(ps->data, ps->size, sizeof(ps->data[0]), Cmp_Name);
	printf("排序成功\n");
}
void FreeContact(struct contact* ps)
{
	free(ps->data);
	ps->data = NULL;
}
void SaveContact(struct contact* ps) 
{
	FILE* pfWrite = fopen("contact.bat", "wb");
	if (pfWrite == NULL)
	{
		printf("%s", strerror(errno));//如果开辟失败，返回错误类型
		return;
	}
	int i = 0;
	for (i = 0; i < ps->size; i++)
	{
		fwrite(&(ps->data[i]), sizeof(struct People_Init), 1, pfWrite);
	}
	printf("保存成功\n");
	fclose(pfWrite);
	pfWrite = NULL;
}