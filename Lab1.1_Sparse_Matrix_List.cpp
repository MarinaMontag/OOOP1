// L/98a+
//Lab1.1_Sprase_Matrix_List.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>

int min(int a, int b);

class Matrix
{
protected:
	int rows;
	int columns;
	int size;
	public:
		Matrix()
		{
			rows = 0;
			columns = 0;
			size = 0;
		}
	virtual void read_matrix(int rows, int columns, std::ifstream& matrix) = 0;
	virtual void print(int columns) = 0;
 
	void Set_rows(int rows)
	{
		this->rows = rows;
	}
	void Set_columns(int columns)
	{
		this->columns = columns;
	}
	int Get_rows()
	{
		return rows;
	}
	int Get_columns()
	{
		return columns;
	}

};

template<typename T>
class Node
{
	T data;
	int freq;
	Node<T>*next;
	Node<T>*prev;
public:
	Node(T data, int freq)
	{
		this->data = data;
		this->freq = freq;
		this->next = nullptr;
		this->prev = nullptr;
	}

};

template<typename T>
class Sparse_List :public Matrix
{
	Node<T>*head = nullptr;
	Node<T>*tail = nullptr;
public:
	
	Sparse_List<T>() : Matrix() {};
	~Sparse_List<T>()
	{
		Node<T>* current = head, *to_delete;
		while (current)
		{
			to_delete = current;
			current = current->next;
			delete to_delete;
		}
	}
	void read_matrix(int rows, int columns, std::ifstream& matrix) 
	{
		std::ifstream file1("matrix.bin", std::ofstream::binary);
		std::vector<T> vect;
		for (int i = 0; i < rows*columns; i++)
		{
			int element;
			matrix.read((char*)&element, sizeof(int));
				if (vect.empty())
					vect.push_back(element);
				else
				{
					if(vect.back()==element)
						vect.push_back(element);
					else {
						matrix.seekg(sizeof(T)*(i-1));
						Node*node(vect.back(), vect.size());
						vect.clear();
						add(node);
						i--;
					}
				}
		}
		matrix.close();
	}
	void print(int columns) 
	{
		int j = 0;
		for (int i = 0; i < size; i++)
		{
				for (int k = 0; k < temp->freq; k++)
				{
					std::cout << temp->data << " ";
					j++;
					if (j == columns)
					{
						std::cout << std::endl;
						j = 0;
					}
				}
				temp = temp->next;
		}
	}
	void add(Node<T>*node)
	{
		if (head == nullptr&&tail == nullptr)
		{
			head = node;
			tail = node;
		}
		else
		{
			Node<T>*temp1 = head;
			while (temp1->next)
				temp1 = temp1->next;
			temp1->next = node;
			node->prev = tail;
			tail = node;
		}
		size++;
	}

	void add_matrixes(Sprase_List<T> A, Sprase_List<T> B)
	{
		Node<T>*temp1 = A.head;
		Node<T>*temp2 = B.head;
		while (temp1 != nullptr && temp2 != nullptr)
		{
			Node<T>*node(temp1->data + temp2->data, min(temp1->freq, temp2->freq));
			if (tail != nullptr)
			{
				if (tail->data != node->data)
					add(node);
				else
					tail->freq += node->freq;
			}
			else
				add(node);
			if (temp1->freq > temp2->freq)
			{
				temp1->freq -= temp2->freq;
				temp2 = temp2->next;
			}
			else if (temp1->freq < temp2->freq)
			{
				temp2->freq -= temp1->freq;
				temp1 = temp1->next;
			}
			else
			{
				temp1 = temp1->next;
				temp2 = temp2->next;
			}
		}
	}
	void multiplication_matrixes(Sprase_List<T> A, Sprase_List<T> B)
	{
		rows = A.rows;
		columns = B.columns;
		if (A.columns != B.rows)
		{
			std::cout << "ERORE!!!" << std::endl;
			return;
		}
		Node<T>*tempA = A.head;
		Node<T>*tempB = B.head;
		for (int i = 0; i < A.rows; i++)
			for (int j = 0; j < B.columns; j++)
			{
				T sum = 0;
				for (int k = 0; k < B.rows; k++)
				{
					sum += tempA.data*tempB.data;
					tempA->freq--;
					if (tempA->freq == 0)
						tempA = temp1->next;
					for (int b = 0; b < B.columns; b++)
					{
						tempB->freq--;
						if (tempB->freq == 0 && temp->next!=nullptr)
							tempB = tempB->next;
					}
				}
		if (tail != nullptr)
		{
			if (tail->data == sum)
				tail->freq++;
			else
				add(sum, 1);
		}
		else add(sum, 1);

		if (i == 0 && j == B.columns - 1)
		{
			tempB = B.head;
			for (int b = 0; b <= j; j++)
			{
				tempB->freq--;
				if (tempB->freq == 0)
					tempB = tempB->next;
			}
			continue;
		}
		else{
			Node<T>*temp = A.head;
			for (int a = 0; a < i; a++)
					for (int l = 0; l < columns; l++)
					{
						temp->freq--;
						if (temp->freq == 0)
							temp = temp->next;
					}
			tempA = temp;
		}

		
		tempB = B.head;
		for (int b = 0; b <= j; j++)
		{
			tempB->freq--;
			if (tempB->freq == 0)
				tempB = tempB->next;
		}
			}
	}
};

template<typename T>
class Sparse_Matrix :public Matrix
{
	std::vector<T> data;
	std::vector<int> row_index;
	std::vector<int>column_index;
public:
	Sparse_Matrix<T>() : Matrix<T>() {};
	void read_matrix(int rows, int columns, std::ifstream& matrix) override
	{
		for(int i=0;i<rows;i++)
			for (int j = 0; j < columns; j++)
			{
				int element;
				matrix.read((char*)&element, sizeof(int));
				if (element != 0)
				{
					data.push_back(element);
					row_index.push_back(i);
					column_index.push_back(j);
					size++;
				}
			}
		matrix.close();
	}
	void print(int rows,int columns) override
	{
		int k = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
				if (k < size&&row_index[k] = i && column_index[k] == j)
				{
					std::cout << data[k] << " ";
					k++
				}
				else
					std::cout << 0 << " ";
			std::cout << std::endl;
		}
	}
void add_matrixes(Sparse_Matrix<T> A, Sparse_Matrix<T> B)
	{
		int a = 0, b = 0;
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				if (A.row_index[a] != i && B.row_index[b] != i && A.column_index[a] != j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] == i && B.row_index[b] != i && A.column_index[a] != j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] != i && B.row_index[b] != i && A.column_index[a] == j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] != i && B.row_index[b] == i && A.column_index[a] != j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] != i && B.row_index[b] != i && A.column_index[a] != j && B.column_index[b] == j)
					continue;
				else if (A.row_index[a] == i && B.row_index[b] == i && A.column_index[a] != j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] != i && B.row_index[b] != i && A.column_index[a] == j && B.column_index[b] == j)
					continue;
				else if (A.row_index[a] == i && B.row_index[b] != i && A.column_index[a] != j && B.column_index[b] == j)
					continue;
				else if (A.row_index[a] != i && B.row_index[b] == i && A.column_index[a] == j && B.column_index[b] != j)
					continue;
				else if (A.row_index[a] == i && B.row_index[b] != i && A.column_index[a] == j && B.column_index[b] != j)
				{
					data.push_back(A.data[a]);
					row_index.push_back(i);
					column_index.push_back(j);
					a++;
				}
				else if (A.row_index[a] != i && B.row_index[b] == i && A.column_index[a] != j && B.column_index[b] == j)
				{
					data.push_back(B.data[b]);
					row_index.push_back(i);
					column_index.push_back(j);
					b++;
				}
				else
				{

					data.push_back(A.data[a] + B.data[b]);
					row_index.push_back(i);
					column_index.push_back(j);
					b++;
					a++;
				}
			}
	}
int elements_in_row(Sparse_Matrix<T> A, int i)
{
	int counter=0
		for (int j = 0; j < A.size; j++)
		{
			if (A.row_index[j] == i)
				counter++;
		}
	return counter;
}
void multiplication_matrixes(Sparse_Matrix<T> A, Sparse_Matrix<T> B)
{
	for (int i = 0; i < A.size; i++)
	{
		int counter = elements_in_row(A, A.row_index[i]);
		for (int k = 0; k < A.rows; k++)
		{
			T sum = 0;
			for (int j = 0; j < counter; j++)
			{
				for(int l=0;l<B.size;l++)
					if (A.column_index[i + j] == B.row_index[l] && B.column_index[l] == k)
					{
						sum += A.data[i + j] * B.data[l];
						break;
				}
			}
			if (sum != 0)
			{
				data.push_back(sum);
				row_index.push_back(A.row_index[i]);
				column_index.push_back(k);
			}
		}
		i += (counter - 1);
	}
	
}

};


int main()
{
	std::ifstream matrix("matrix.bin", std::ifstream::binary);
	Sparse_List<int> A;
}

int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.


