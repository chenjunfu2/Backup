#pragma once

#include "Error.hpp"
#include <new>

template<typename DataType>
class List
{
public:
	struct Node
	{
		Node *pPrev;//��һ���ڵ�
		Node *pNext;//��һ���ڵ�
		DataType tData;//����
	};

	class Iterator//������
	{
	private:
		Node *pCurrent;
	public:
		Iterator(Node *_pcurrent) :pCurrent(_pcurrent)
		{}

		Iterator &operator++(void)//��int��ǰ׺����
		{
			pCurrent = pCurrent->pNext;
			return *this;
		}

		Iterator &operator--(void)//��int��ǰ׺�ݼ�
		{
			pCurrent = pCurrent->pPrev;
			return *this;
		}

		Node &operator*(void)
		{
			return *pCurrent;
		}

		bool operator!=(Iterator &_rRight)
		{
			return pCurrent != _rRight.pCurrent;
		}
	};
private:
	Node *pHead;//ͷ��
	Node *pTail;//β��
public:
	List(void) :pHead(nullptr), pTail(nullptr)
	{}

	~List(void)
	{
		RemoveAll();
	}

	void RemoveAll(void)
	{
		Node *pDel = pHead;
		while (pDel != nullptr)//����ɾ������Ԫ��
		{
			pHead = pHead->pNext;
			delete pDel;
			pDel = pHead;
		}
		pHead = nullptr;
		pTail = nullptr;
	}

	Error InsertHead(const DataType &tData)
	{
		//���䲢��ʼ��
		Node *pNewNode = new(std::nothrow) Node{nullptr,pHead,tData};
		if (pNewNode == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "�ڴ治��");
		}

		//����ͷ�ڵ�ָ��ǰһ���ڵ��ָ��Ϊ��ǰ����Ľڵ�
		if (pHead != nullptr)
		{
			pHead->pPrev = pNewNode;
		}

		//����ͷ�ڵ�Ϊ�½ڵ�
		pHead = pNewNode;

		//����β�ڵ�
		if (pTail == nullptr)
		{
			pTail = pNewNode;
		}

		return Error(false);
	}

	Error InsertTail(const DataType &tData)
	{
		//���䲢��ʼ��
		Node *pNewNode = new(std::nothrow) Node{pTail,nullptr,tData};
		if (pNewNode == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "�ڴ治��");
		}

		//����β�ڵ�ĺ�һ���ڵ�ָ��Ϊ��ǰ����Ľڵ�
		if (pTail != nullptr)
		{
			pTail->pNext = pNewNode;
		}

		//����β�ڵ�Ϊ�½ڵ�
		pTail = pNewNode;

		//����ͷ�ڵ�
		if (pHead == nullptr)
		{
			pHead = pNewNode;
		}

		return Error(false);
	}

	Error RemoveHead(void)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "����ͷΪ��");
		}

		//����ͷ��ָ��
		Node *pDelNode = pHead;

		//����ָ��
		pHead = pHead->pNext;
		if (pHead != nullptr)//����Ԫ��
		{
			pHead->pPrev = nullptr;//ȡ����ǰһ��Ԫ��ָ�������
		}
		else//ɾ����
		{
			pTail = nullptr;//β��ָ��ҲҪȡ�������һ��Ԫ�ص�����
		}

		//ɾ��
		delete pDelNode;
		pDelNode = nullptr;

		return Error(false);
	}

	Error RemoveTail(void)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "����βΪ��");
		}

		//����β��ָ��
		Node *pDelNode = pTail;

		//����ָ��
		pTail = pTail->pPrev;
		if (pTail != nullptr)//����Ԫ��
		{
			pTail->pNext = nullptr;//ȡ���Ժ�һ��Ԫ��ָ�������
		}
		else//ɾ����
		{
			pHead = nullptr;//ͷ��ָ��ҲҪȡ�������һ��Ԫ�ص�����
		}

		//ɾ��
		delete pDelNode;
		pDelNode = nullptr;

		return Error(false);
	}

	Error MoveHeadTo(List &rTargetList)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����ͷΪ��");
		}

		//����ͷ��ָ��
		Node *pDelNode = pHead;

		//��Դ������ɾ��Ŀ��Ԫ�ص����ͷ�
		pHead = pHead->pNext;
		if (pHead != nullptr)//����Ԫ��
		{
			pHead->pPrev = nullptr;//ȡ����ǰһ��Ԫ��ָ�������
		}
		else//ɾ����
		{
			pTail = nullptr;//β��ָ��ҲҪȡ�������һ��Ԫ�ص�����
		}

		//����ָ��
		Node *&pNewNode = pDelNode;

		pNewNode->pPrev = nullptr;
		pNewNode->pNext = rTargetList.pHead;

		//��ӵ�Ŀ������
		//����ͷ�ڵ�ָ��ǰһ���ڵ��ָ��Ϊ��ǰ�Ľڵ�
		if (rTargetList.pHead != nullptr)
		{
			rTargetList.pHead->pPrev = pNewNode;
		}

		//����ͷ�ڵ�Ϊ�½ڵ�
		rTargetList.pHead = pNewNode;

		//����β�ڵ�
		if (rTargetList.pTail == nullptr)
		{
			rTargetList.pTail = pNewNode;
		}

		return Error(false);
	}

	Error MoveTailTo(List &rTargetList)
	{
		if (pTail == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����βΪ��");
		}

		//����β��ָ��
		Node *pDelNode = pTail;

		//��Դ������ɾ��Ŀ��Ԫ�ص����ͷ�
		pTail = pTail->pPrev;
		if (pTail != nullptr)//����Ԫ��
		{
			pTail->pNext = nullptr;//ȡ���Ժ�һ��Ԫ��ָ�������
		}
		else//ɾ����
		{
			pHead = nullptr;//ͷ��ָ��ҲҪȡ�������һ��Ԫ�ص�����
		}

		//����ָ��
		Node *&pNewNode = pDelNode;

		pNewNode->pPrev = rTargetList.pTail;
		pNewNode->pNext = nullptr;

		//��ӵ�Ŀ������
		//����β�ڵ�ָ���һ���ڵ��ָ��Ϊ��ǰ�Ľڵ�
		if (rTargetList.pTail != nullptr)
		{
			rTargetList.pTail->pNext = pNewNode;
		}

		//����β�ڵ�Ϊ�½ڵ�
		rTargetList.pTail = pNewNode;

		//����ͷ�ڵ�
		if (rTargetList.pHead == nullptr)
		{
			rTargetList.pHead = pNewNode;
		}

		return Error(false);
	}

	DataType *GetHead(void)
	{
		if (pHead == nullptr)
		{
			return nullptr;
		}
		return &pHead->tData;
	}

	DataType *GetTail(void)
	{
		if (pTail == nullptr)
		{
			return nullptr;
		}
		return &pTail->tData;
	}

	Iterator begin(void)
	{
		return Iterator(pHead);
	}

	Iterator rbegin(void)
	{
		return Iterator(pTail);
	}

	Iterator end(void)
	{
		return Iterator(nullptr);
	}

	Iterator rend(void)
	{
		return Iterator(nullptr);
	}
};