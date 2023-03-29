#pragma once

#include "Error.hpp"
#include <new>
#include <initializer_list>

template<typename DataType>
class List
{
private:
	//�ڵ�����
	struct Node
	{
		Node *pPrev;//��һ���ڵ�
		Node *pNext;//��һ���ڵ�
		DataType tData;//����
	};

	Node *pHead;//ͷ��
	Node *pTail;//β��
	size_t szNodeNum;//Ԫ�ظ�������
public:
	//������
	class Iterator//������
	{
	private:
		friend class List;//��������Ϊ��Ԫ��
		Node *pCurrent;
	protected:
		//����(ֻ��������ͼ̳���ʹ��Node����)
		Iterator(Node *_pcurrent = nullptr) : pCurrent(_pcurrent)
		{}
	public:
		//��������
		Iterator(const Iterator &_Iterator) :pCurrent(_Iterator.pCurrent)
		{}

		//�ƶ�����
		Iterator(Iterator &&_Iterator) :pCurrent(_Iterator.pCurrent)
		{
			_Iterator.pCurrent = nullptr;
		}

		//Ĭ��������ƽ�������ࣩ
		~Iterator(void) = default;

		//ǰ׺����
		Iterator &operator++(void)//��int��ǰ׺����
		{
			if (pCurrent != nullptr)
			{
				pCurrent = pCurrent->pNext;
			}
			return *this;
		}

		//ǰ׺�ݼ�
		Iterator &operator--(void)//��int��ǰ׺�ݼ�
		{
			if (pCurrent != nullptr)
			{
				pCurrent = pCurrent->pPrev;
			}
			return *this;
		}

		//��׺����
		Iterator operator++(int)//��int�Ǻ�׺����
		{
			Node *pLast = pCurrent;//����ԭ����
			if (pCurrent != nullptr)
			{
				pCurrent = pCurrent->pNext;//ǰ������һ��
			}
			return pLast;//����ԭ����
		}

		//��׺�ݼ�
		Iterator operator--(int)//��int�Ǻ�׺�ݼ�
		{
			Node *pLast = pCurrent;//����ԭ����
			if (pCurrent != nullptr)
			{
				pCurrent = pCurrent->pPrev;//���˵���һ��
			}
			return pLast;//����ԭ����
		}

		//�ӵ�ǰ������_szAdd����
		Iterator operator+(size_t _szAdd) const
		{
			Node *pAddNode = pCurrent;//����ԭ���Ĳ���
			while (pAddNode != nullptr)//���ε���
			{
				if (_szAdd == 0)
				{
					break;//�ҵ�Ҫ����
				}
				//�ݼ�Ȼ�������һ��Ԫ��
				--_szAdd;
				pAddNode = pAddNode->pNext;
			}
			return pAddNode;//�������µ�
		}

		//�ӵ�ǰ����ǰ_szDel����
		Iterator operator-(size_t _szDel) const
		{
			Node *pRemove = pCurrent;//����ԭ���Ĳ���
			while (pRemove != nullptr)//���ε���
			{
				if (_szDel == 0)
				{
					break;//�ҵ�Ҫ����
				}
				//�ݼ�Ȼ�������һ��Ԫ��
				--_szDel;
				pRemove = pRemove->pPrev;
			}
			return pRemove;//�������µ�
		}

		//�����û�ȡData
		DataType &operator*(void) const
		{
			return pCurrent->tData;//���ж��ˣ�����û�������nullptrֱ���ó���ը
		}
		
		//�����ж�
		bool operator!=(Iterator &_rRight) const
		{
			return pCurrent != _rRight.pCurrent;
		}
		
		//����ж�
		bool operator==(Iterator &_rRight) const
		{
			return pCurrent == _rRight.pCurrent;
		}

		//��������Ч�Լ�⣨���pCurrent�ǲ���nullptr��
		operator bool(void) const
		{
			return pCurrent != nullptr;
		}

		//��������Ч�Լ�⣨���pCurrent�ǲ���nullptr��
		bool operator!(void) const
		{
			return pCurrent == nullptr;
		}
	};

	//Ĭ���޲ι���
	List(void) :pHead(nullptr), pTail(nullptr), szNodeNum(0)
	{}

	//��һ��dataԪ�ع���
	List(const DataType &_tData) :pHead(nullptr), pTail(nullptr), szNodeNum(0)
	{
		InsertTail(_tData);//����
	}

	//��һ����ʼ���б���
	List(const std::initializer_list<DataType> &_tInitDataList) :pHead(nullptr), pTail(nullptr), szNodeNum(0)
	{
		for (auto &it : _tInitDataList)//���������б�
		{
			if (!InsertTail(it))//���β���
			{
				return;//ʧ����ǰ����
			}
		}
	}

	//��������
	List(const List &_List) :pHead(nullptr), pTail(nullptr), szNodeNum(0)
	{
		Node *pAdd = _List.pHead;
		while (pAdd != nullptr)//���������ṹ
		{
			if (!InsertTail(pAdd->tData))//���뵽����
			{
				break;//ʧ�ܷ���
			}
			pAdd = pAdd->pNext;//������һ��
		}
	}

	//�ƶ�����
	List(List &&_List) :pHead(_List.pHead), pTail(_List.pTail), szNodeNum(_List.szNodeNum)
	{
		_List.pHead = nullptr;
		_List.pTail = nullptr;
		_List.szNodeNum = 0;
	}

	//����
	~List(void)
	{
		RemoveAll();
	}

	//��ֵ
	List &operator=(const List &_List)
	{
		RemoveAll();
		Node *pAdd = _List.pHead;
		while (pAdd != nullptr)//���������ṹ
		{
			if (!InsertTail(pAdd->tData))//���뵽����
			{
				break;//ʧ�ܷ���
			}
			pAdd = pAdd->pNext;//������һ��
		}
		return *this;
	}

	//ɾ������Ԫ�أ��������
	Error RemoveAll(void)
	{
		if (pHead == nullptr && pTail == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "�����Ѿ�Ϊ��");
		}
		else if (pHead != nullptr && pTail == nullptr)//��ͬ��
		{
			return Error(true, 0, __FUNCTION__, "�������ݲ�ͬ��");
		}
		else if (pHead == nullptr && pTail != nullptr)//��ͬ��
		{
			return Error(true, 0, __FUNCTION__, "�������ݲ�ͬ��");
		}

		Node *pDel = pHead;
		while (pDel != nullptr)//����ɾ������Ԫ��
		{
			pHead = pHead->pNext;
			delete pDel;
			pDel = pHead;
		}
		
		//�ÿ�����
		pHead = nullptr;
		pTail = nullptr;
		szNodeNum = 0;

		return Error(false);
	}

	//�Ƿ�Ϊ��
	bool IsEmpty(void)
	{
		return pHead == pTail == nullptr;
	}

	//��ȡNode����
	size_t GetNodeNum(void) const
	{
		return szNodeNum;
	}

	//ͷ��
	Error InsertHead(const DataType &tData)
	{
		//���䲢��ʼ��
		Node *pInsert = new(std::nothrow) Node{nullptr,pHead,tData};
		if (pInsert == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "�ڴ治��");
		}

		//����ͷ�ڵ�ָ��ǰһ���ڵ��ָ��Ϊ��ǰ����Ľڵ�
		if (pHead != nullptr)
		{
			pHead->pPrev = pInsert;
		}

		//����ͷ�ڵ�Ϊ�½ڵ�
		pHead = pInsert;

		//����β�ڵ�
		if (pTail == nullptr)
		{
			pTail = pInsert;
		}

		//����Ԫ�ؼ���
		++szNodeNum;

		return Error(false);
	}

	//β��
	Error InsertTail(const DataType &tData)
	{
		//���䲢��ʼ��
		Node *pInsert = new(std::nothrow) Node{pTail,nullptr,tData};
		if (pInsert == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "�ڴ治��");
		}

		//����β�ڵ�ĺ�һ���ڵ�ָ��Ϊ��ǰ����Ľڵ�
		if (pTail != nullptr)
		{
			pTail->pNext = pInsert;
		}

		//����β�ڵ�Ϊ�½ڵ�
		pTail = pInsert;

		//����ͷ�ڵ�
		if (pHead == nullptr)
		{
			pHead = pInsert;
		}

		//����Ԫ�ؼ���
		++szNodeNum;

		return Error(false);
	}

	//�м����(��itposָ���Ԫ�غ����)
	Error InsertMid(const DataType &tData, Iterator &itPos)
	{
		Node *pInsert = itPos.pCurrent;//�����λ�ú����
		if (pInsert == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "������Ϊ��");
		}

		//���䲢��ʼ��
		Node *pInsert = new(std::nothrow) Node{pInsert,pInsert->pNext,tData};
		if (pInsert == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "�ڴ治��");
		}

		//���������һ���ڵ㣬��ô��һ���ڵ����һ���ڵ�Ϊ�½ڵ�
		if (pInsert->pNext != nullptr)
		{
			pInsert->pNext->pPrev = pInsert;
		}
		else//�����������һ���ڵ㣬����ǰ�ڵ���β�ڵ�
		{
			pTail = pInsert;//����β�ڵ�Ϊ�²���Ľڵ�
		}

		//���ò�������һ���ڵ�Ϊ�½ڵ�
		pInsert->pNext = pInsert;

		//����Ԫ�ؼ���
		++szNodeNum;

		//����itPosʹ��ָ���²���Ľڵ�
		++itPos;

		return Error(false);
	}

	//ͷɾ
	Error RemoveHead(void)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "����ͷΪ��");
		}

		//����ͷ��ָ��
		Node *pRemove = pHead;

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
		delete pRemove;
		pRemove = nullptr;

		//�ݼ�Ԫ�ؼ���
		--szNodeNum;

		return Error(false);
	}

	//βɾ
	Error RemoveTail(void)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "����βΪ��");
		}

		//����β��ָ��
		Node *pRemove = pTail;

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
		delete pRemove;
		pRemove = nullptr;

		//�ݼ�Ԫ�ؼ���
		--szNodeNum;

		return Error(false);
	}

	//�м�ɾ��
	Error RemoveMid(Iterator &itPos)
	{
		Node *pRemove = itPos.pCurrent;
		if (pRemove == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "������Ϊ��");
		}

		//����������һ��Ԫ��
		++itPos;

		//����ǰ�ڵ�ָ���һ���ڵ�ָ��ΪҪɾ���ڵ��һ���ڵ��ָ��
		if (pRemove->pPrev != nullptr)
		{
			pRemove->pPrev->pNext = pRemove->pNext;
		}
		else//Ҫɾ���ڵ���ͷ�ڵ�
		{
			pHead = pRemove->pNext;//����ͷ�ڵ�ָ��Ҫɾ���ڵ����һ���ڵ�
		}

		//���ú�ڵ�ָ��ǰһ���ڵ��ָ��ΪҪɾ���ڵ�ǰһ���ڵ��ָ��
		if (pRemove->pNext != nullptr)
		{
			pRemove->pNext->pPrev = pRemove->pPrev;
		}
		else//Ҫɾ���ڵ���β�ڵ�
		{
			pTail = pRemove->pPrev;//����β�ڵ�ָ��Ҫɾ���ڵ����һ���ڵ�
		}
		
		//ɾ��
		delete pRemove;
		pRemove = nullptr;

		//�ݼ�Ԫ�ؼ���
		--szNodeNum;

		return Error(false);
	}

	//�ƶ�ͷ����Ŀ��ṹ
	Error MoveHeadToHead(List &rTargetList)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����ͷΪ��");
		}

		//����ͷ��ָ��
		Node *pRemove = pHead;

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
		Node *&pInsert = pRemove;

		pInsert->pPrev = nullptr;
		pInsert->pNext = rTargetList.pHead;

		//��ӵ�Ŀ������
		//����ͷ�ڵ�ָ��ǰһ���ڵ��ָ��Ϊ��ǰ�Ľڵ�
		if (rTargetList.pHead != nullptr)
		{
			rTargetList.pHead->pPrev = pInsert;
		}

		//����ͷ�ڵ�Ϊ�½ڵ�
		rTargetList.pHead = pInsert;

		//����β�ڵ�
		if (rTargetList.pTail == nullptr)
		{
			rTargetList.pTail = pInsert;
		}

		//�ݼ�Ԫ�ؼ���
		--szNodeNum;

		return Error(false);
	}

	//�ƶ�β����Ŀ��ṹ
	Error MoveTailToTail(List &rTargetList)
	{
		if (pTail == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����βΪ��");
		}

		//����β��ָ��
		Node *pRemove = pTail;

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
		Node *&pInsert = pRemove;

		pInsert->pPrev = rTargetList.pTail;
		pInsert->pNext = nullptr;

		//��ӵ�Ŀ������
		//����β�ڵ�ָ���һ���ڵ��ָ��Ϊ��ǰ�Ľڵ�
		if (rTargetList.pTail != nullptr)
		{
			rTargetList.pTail->pNext = pInsert;
		}

		//����β�ڵ�Ϊ�½ڵ�
		rTargetList.pTail = pInsert;

		//����ͷ�ڵ�
		if (rTargetList.pHead == nullptr)
		{
			rTargetList.pHead = pInsert;
		}

		//�ݼ�Ԫ�ؼ���
		--szNodeNum;

		return Error(false);
	}

	//��ȡͷ��һ��
	Error GetHead(DataType &tDataGet)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����ͷΪ��");
		}

		//������Ŀ��
		tDataGet = pHead->tData;

		return Error(false);
	}

	//��ȡβ��һ��
	Error GetTail(DataType &tDataGet)
	{
		if (pTail == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "Դ����βΪ��");
		}

		//������Ŀ��
		tDataGet = pTail->tData;

		return Error(false);
	}

	//��������ʼ
	Iterator begin(void) const
	{
		return Iterator(pHead);
	}

	//����������
	Iterator end(void) const
	{
		return Iterator(nullptr);
	}

	//�����������ʼ
	Iterator rbegin(void) const
	{
		return Iterator(pTail);
	}

	//�������������
	Iterator rend(void) const
	{
		return Iterator(nullptr);
	}
};