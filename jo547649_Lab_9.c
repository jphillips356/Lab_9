#include <stdio.h>
#include <stdlib.h> // For malloc and exit

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// Linked list node
struct Node
{
    struct RecordType data;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
    struct Node** hashArray; // Array of pointers to Node
    int size; // Size (duh)
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// Displays records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = pHashArray->hashArray[i];
        while (current != NULL)
        {
            printf("%d %c %d", current->data.id, current->data.name, current->data.order);
            current = current->next;
            if (current != NULL)
                printf(" -> ");
        }
        printf("\n");
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    // Initialize the hash table
    int hashSz = recordSz * 2; 
    struct HashType hashTable;
    hashTable.hashArray = (struct Node**)malloc(sizeof(struct Node*) * hashSz);
    if (hashTable.hashArray == NULL)
    {
        printf("Cannot allocate memory for the hash table\n");
        exit(-1);
    }
    hashTable.size = hashSz;

    // Initialize each element of the hash table as NULL (empty linked list)
    for (int i = 0; i < hashSz; ++i)
    {
        hashTable.hashArray[i] = NULL;
    }

    // Insert records into the hash table using chaining
    for (int i = 0; i < recordSz; ++i)
    {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        if (newNode == NULL)
        {
            printf("Cannot allocate memory for a new node\n");
            exit(-1);
        }
        newNode->data = pRecords[i];

        int index = hash(pRecords[i].id, hashSz);
        newNode->next = hashTable.hashArray[index];
        hashTable.hashArray[index] = newNode;
    }

    // Display records in the hash structure
    displayRecordsInHash(&hashTable, hashSz);

    // Free the allocated memory for the hash table and records
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* current = hashTable.hashArray[i];
        while (current != NULL)
        {
            struct Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable.hashArray);
    free(pRecords);

    return 0;
}
