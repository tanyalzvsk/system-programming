
/*************************************************************************
   LAB 1

	Edit this file ONLY!

*************************************************************************/

#pragma warning(disable : 4996)
#include "dns.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

unsigned short HashCode(const char* S);

struct Element* InitElement(IPADDRESS IP, const char* DN);

void AddElementToList(struct ListOfElements* list, IPADDRESS IP, const char* DN);

struct ListOfElements//один из листов
{
	struct Element* FirstElement;
	struct Element* LastElement;
};

struct Element
{
	IPADDRESS IP;
	char* DN;//Domain Name
	struct Element* nextElement;
};

void AddElementToHashTable(DNSHandle hdns, IPADDRESS IP, const char* DN)
{
	unsigned short hash = HashCode(DN);
	struct ListOfElements** engine = (struct ListOfElements**)hdns;
	AddElementToList(engine[(unsigned int)hash], IP, DN);
}

void AddElementToList(struct ListOfElements* list, IPADDRESS IP, const char* DN)
{
	if (list->FirstElement == NULL)
	{
		list->FirstElement = InitElement(IP, DN);
		list->LastElement = list->FirstElement;
	}
	else
	{
		list->LastElement->nextElement = InitElement(IP, DN);
		list->LastElement = list->LastElement->nextElement;
	}
}

struct Element* InitElement(IPADDRESS IP, const char* DN)
{
	struct Element* element = (struct Element*)malloc(sizeof(struct Element));
	if (element != NULL)
	{
		element->IP = IP;
		element->DN = (char*)malloc(strlen(DN) + 1);
		if (element->DN != NULL)
			strcpy(element->DN, DN);
		element->nextElement = NULL;
	}
	return element;
}

unsigned short HashCode(const char* S)
{
	unsigned short tmp = 0;
	while (*S)
	{
		tmp += *S++;
	}
	return tmp;
}

DNSHandle InitDNS()
{
	struct ListOfElements** engine = (struct ListOfElements**)malloc(sizeof(struct ListOfElements*) * 65536);
	if (engine != NULL)
	{
		for (int i = 0; i < 65536; i++)
		{
			engine[i] = (struct ListOfElements*)malloc(sizeof(struct ListOfElements));
			if (engine[i] != NULL)
			{
				engine[i]->FirstElement = NULL;
				engine[i]->LastElement = NULL;
			}
		}
		return (DNSHandle)engine;
	}
	else
		return (DNSHandle)INVALID_DNS_HANDLE;
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
	FILE* fin = fopen(hostsFilePath, "r");

	int firstN, secondN, thirdN, fourthN;
	char DN[256];//domain name
	while (fscanf(fin, "%d.%d.%d.%d %s", &firstN, &secondN, &thirdN, &fourthN, DN) != EOF)
	{
		IPADDRESS ip = firstN << 24 | secondN << 16 | thirdN << 8 | fourthN;
		AddElementToHashTable(hDNS, ip, DN);
	}
	fclose(fin);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
	unsigned short index = HashCode(hostName);
	struct ListOfElements* list = ((struct ListOfElements**)hDNS)[index];
	struct Element* element = list->FirstElement;
	while (element != NULL && strcmp(element->DN, hostName) != 0)
	{
		element = element->nextElement;
	}
	if (element != NULL)
		return element->IP;
	return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
	if (hDNS != NULL)
	{
		struct ListOfElements** dns = (struct ListOfElements**)hDNS;
		for (int i = 0; i < 65536; i++)
		{
			if (dns[i] == NULL)
				continue;

			struct Element* element = dns[i]->FirstElement;

			while (element != NULL)
			{
				struct Element* tmp = element;
				element = element->nextElement;
				free(tmp->DN);
				free(tmp);
			}

			free(dns[i]);
		}
		
		free(dns);
	}
}
