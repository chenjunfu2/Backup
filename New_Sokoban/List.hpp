#pragma once

#include "Error.hpp"
#include <new>

template<typename DataType>
class List
{
public:
	struct Node
	{
		Node *pPrev;//上一个节点
		Node *pNext;//下一个节点
		DataType tData;//数据
	};

	class Iterator//迭代器
	{
	private:
		Node *pCurrent;
	public:
		Iterator(Node *_pcurrent) :pCurrent(_pcurrent)
		{}

		Iterator &operator++(void)//无int是前缀递增
		{
			pCurrent = pCurrent->pNext;
			return *this;
		}

		Iterator &operator--(void)//无int是前缀递减
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
	Node *pHead;//头部
	Node *pTail;//尾部
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
		while (pDel != nullptr)//遍历删除所有元素
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
		//分配并初始化
		Node *pNewNode = new(std::nothrow) Node{nullptr,pHead,tData};
		if (pNewNode == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "内存不足");
		}

		//设置头节点指向前一个节点的指针为当前分配的节点
		if (pHead != nullptr)
		{
			pHead->pPrev = pNewNode;
		}

		//设置头节点为新节点
		pHead = pNewNode;

		//设置尾节点
		if (pTail == nullptr)
		{
			pTail = pNewNode;
		}

		return Error(false);
	}

	Error InsertTail(const DataType &tData)
	{
		//分配并初始化
		Node *pNewNode = new(std::nothrow) Node{pTail,nullptr,tData};
		if (pNewNode == nullptr)
		{
			return Error(true, 0, __FUNCTION__, "内存不足");
		}

		//设置尾节点的后一个节点指针为当前分配的节点
		if (pTail != nullptr)
		{
			pTail->pNext = pNewNode;
		}

		//设置尾节点为新节点
		pTail = pNewNode;

		//设置头节点
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
			return Error(true, 1, __FUNCTION__, "链表头为空");
		}

		//保存头部指针
		Node *pDelNode = pHead;

		//设置指针
		pHead = pHead->pNext;
		if (pHead != nullptr)//还有元素
		{
			pHead->pPrev = nullptr;//取消对前一个元素指针的引用
		}
		else//删完了
		{
			pTail = nullptr;//尾部指针也要取消对最后一个元素的引用
		}

		//删除
		delete pDelNode;
		pDelNode = nullptr;

		return Error(false);
	}

	Error RemoveTail(void)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "链表尾为空");
		}

		//保存尾部指针
		Node *pDelNode = pTail;

		//设置指针
		pTail = pTail->pPrev;
		if (pTail != nullptr)//还有元素
		{
			pTail->pNext = nullptr;//取消对后一个元素指针的引用
		}
		else//删完了
		{
			pHead = nullptr;//头部指针也要取消对最后一个元素的引用
		}

		//删除
		delete pDelNode;
		pDelNode = nullptr;

		return Error(false);
	}

	Error MoveHeadTo(List &rTargetList)
	{
		if (pHead == nullptr)
		{
			return Error(true, 1, __FUNCTION__, "源链表头为空");
		}

		//保存头部指针
		Node *pDelNode = pHead;

		//从源链表中删除目标元素但不释放
		pHead = pHead->pNext;
		if (pHead != nullptr)//还有元素
		{
			pHead->pPrev = nullptr;//取消对前一个元素指针的引用
		}
		else//删完了
		{
			pTail = nullptr;//尾部指针也要取消对最后一个元素的引用
		}

		//设置指针
		Node *&pNewNode = pDelNode;

		pNewNode->pPrev = nullptr;
		pNewNode->pNext = rTargetList.pHead;

		//添加到目标链表
		//设置头节点指向前一个节点的指针为当前的节点
		if (rTargetList.pHead != nullptr)
		{
			rTargetList.pHead->pPrev = pNewNode;
		}

		//设置头节点为新节点
		rTargetList.pHead = pNewNode;

		//设置尾节点
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
			return Error(true, 1, __FUNCTION__, "源链表尾为空");
		}

		//保存尾部指针
		Node *pDelNode = pTail;

		//从源链表中删除目标元素但不释放
		pTail = pTail->pPrev;
		if (pTail != nullptr)//还有元素
		{
			pTail->pNext = nullptr;//取消对后一个元素指针的引用
		}
		else//删完了
		{
			pHead = nullptr;//头部指针也要取消对最后一个元素的引用
		}

		//设置指针
		Node *&pNewNode = pDelNode;

		pNewNode->pPrev = rTargetList.pTail;
		pNewNode->pNext = nullptr;

		//添加到目标链表
		//设置尾节点指向后一个节点的指针为当前的节点
		if (rTargetList.pTail != nullptr)
		{
			rTargetList.pTail->pNext = pNewNode;
		}

		//设置尾节点为新节点
		rTargetList.pTail = pNewNode;

		//设置头节点
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