#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "List.hpp"
#include "Record.hpp"


//�����߼�
class Game_Control
{
public:
	struct Move_Data
	{
		signed char cXMove, cYMove;//XY�ƶ�����
		unsigned char ucCode;//������
	};

	struct File
	{
		List<Move_Data> &csOperate;//������¼
		List<Move_Data> &csUndo;//������¼
	};
private:
	Map &csMap;
	Player &csPlayer;
	Record &csRecord;
	List<Move_Data> csOperate;//������¼
	List<Move_Data> csUndo;//������¼
public:
	Game_Control(Map &_csMap, Player &_csPlayer, Record &_csRecord, const File &_File) :
		csMap(_csMap), csPlayer(_csPlayer), csRecord(_csRecord), csOperate(std::move(_File.csOperate)), csUndo(std::move(_File.csUndo))
	{}

	const File GetFile(void)
	{
		return File(csOperate, csUndo);
	}
public:
	Game_Control(Map &_csMap, Player &_csPlayer, Record &_csRecord) :
		csMap(_csMap), csPlayer(_csPlayer), csRecord(_csRecord)
	{}
	~Game_Control(void) = default;

	bool IsWin(void)
	{
		return csMap.DestnBoxNum() == csMap.AllBoxNum();
	}

	bool MoveUp(void)//��
	{
		return MovePlayer(0, -1);
	}

	bool MoveDn(void)//��
	{
		return MovePlayer(0, 1);
	}

	bool MoveLf(void)//��
	{
		return MovePlayer(-1, 0);
	}

	bool MoveRi(void)//��
	{
		return MovePlayer(1, 0);
	}

	//�ƶ�
	bool MovePlayer(signed char cXMove, signed char cYMove)
	{
		// Խ���ж�
		if (csPlayer.x + cXMove < 0 || csPlayer.x + cXMove >= csMap.Width() ||
			csPlayer.y + cYMove < 0 || csPlayer.y + cYMove >= csMap.Hight())
		{
			return false;
		}

		//��õ�ǰ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + cXMove, csPlayer.y + cYMove);

		if (enPlayerMoveBlock == Map::Wall)//���ǰ����ǽ��
		{
			return false;
		}
		else if (enPlayerMoveBlock == Map::Blank)//���ǰ���ǿյ�
		{
			//�������״̬
			csPlayer.enPlayerStatus = Player::PlayerInBlank;
			//��������
			csOperate.InsertTail({cXMove,cYMove,0});
		}
		else if (enPlayerMoveBlock == Map::Destn)//���ǰ����Ŀ�ĵ�
		{
			//�������״̬
			csPlayer.enPlayerStatus = Player::PlayerInDestn;
			//��������
			csOperate.InsertTail({cXMove,cYMove,1});
		}
		else//�������
		{
			//Խ���ж�
			if (csPlayer.x + cXMove * 2 < 0 || csPlayer.x + cXMove * 2 >= csMap.Width() ||
				csPlayer.y + cYMove * 2 < 0 || csPlayer.y + cYMove * 2 >= csMap.Hight())
			{
				return false;
			}

			//�����Ҷ����ƶ������ϵĵ�ͼʵ��
			Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + cXMove * 2, csPlayer.y + cYMove * 2);

			if (enPlayerMoveBlock == Map::BoxInBlank)//���ǰ�����ڿյ��ϵ�����
			{
				if (enPlayerDoubleMoveBlock == Map::Blank)//����ǰ���ǿյ�
				{
					//�ƶ���
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//�������״̬
					csPlayer.enPlayerStatus = Player::PlayerInBlank;
					//��������
					csOperate.InsertTail({cXMove,cYMove,2});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//����ǰ����Ŀ�ĵ�
				{
					//����Ŀ�ĵ�������
					csMap.IncDestnBoxNum();
					//�ƶ���
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//�������״̬
					csPlayer.enPlayerStatus = Player::PlayerInBlank;
					//��������
					csOperate.InsertTail({cXMove,cYMove,3});
				}
				else//δ���������ǰ���ǣ����ӡ���Ŀ�ĵ��ϵ����ӡ�ǽ�ڣ�ֱ�ӷ���
				{
					return false;
				}
			}
			else if (enPlayerMoveBlock == Map::BoxInDestn)//���ǰ������Ŀ�ĵ��ϵ�����
			{
				if (enPlayerDoubleMoveBlock == Map::Blank)//����ǰ���ǿյ�
				{
					//�ݼ�Ŀ�ĵ�������
					csMap.DecDestnBoxNum();
					//�ƶ���
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//�������״̬
					csPlayer.enPlayerStatus = Player::PlayerInDestn;
					//��������
					csOperate.InsertTail({cXMove,cYMove,4});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//����ǰ����Ŀ�ĵ�
				{
					//�ƶ���
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//�������״̬
					csPlayer.enPlayerStatus = Player::PlayerInDestn;
					//��������
					csOperate.InsertTail({cXMove,cYMove,5});
				}
				else//δ���������ǰ���ǣ����ӡ���Ŀ�ĵ��ϵ����ӡ�ǽ�ڣ�ֱ�ӷ���
				{
					return false;
				}
			}
			else//���������ֱ�ӷ���
			{
				return false;
			}
		}

		//�ƶ����
		csPlayer.x += cXMove;
		csPlayer.y += cYMove;
		++csRecord;//�����ƶ�����

		csUndo.RemoveAll();//��ճ������������Ԫ�صĻ���

		return true;
	}

	//�����ƶ�
	bool UndoMove(void)
	{
		//��ȡβ��һ��
		Move_Data stMoveData;
		if (!csOperate.GetTail(stMoveData))
		{
			return false;
		}
		//����鿴�ɹ�����β��һ����Ԫ�أ��˴������ٴ��ж�
		csOperate.MoveTailToTail(csUndo);//ת�Ƶ�����������


		switch (stMoveData.ucCode)
		{
		case 0:
		case 1:
			break;//�޵�ͼ������ɶ������
		default:
			{
				// Խ���ж�
				if (csPlayer.x + stMoveData.cXMove < 0 || csPlayer.x + stMoveData.cXMove >= csMap.Width() ||
					csPlayer.y + stMoveData.cYMove < 0 || csPlayer.y + stMoveData.cYMove >= csMap.Hight())
				{
					return false;
				}

				//�������ƶ������ϵĵ�ͼʵ��
				Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.cXMove, csPlayer.y + stMoveData.cYMove);
				//�����ҵ�ǰλ���ϵĵ�ͼʵ��
				Map::Block &enPlayerCurrentBlock = csMap(csPlayer.x, csPlayer.y);

				switch (stMoveData.ucCode)
				{
				case 2:
					{
						//�������������ƶ���������ϣ����λ���ǿյأ����ָ�ԭ�ȵĿյ�
						enPlayerMoveBlock = Map::Blank;
						enPlayerCurrentBlock = Map::BoxInBlank;
					}
					break;
				case 3:
					{
						//�ݼ�Ŀ�ĵ�������
						csMap.DecDestnBoxNum();
						//�������������ƶ���������ϣ����λ���ǿյأ����ָ�ԭ�ȵ�Ŀ�ĵ�
						enPlayerMoveBlock = Map::Destn;
						enPlayerCurrentBlock = Map::BoxInBlank;
					}
					break;
				case 4:
					{
						//����Ŀ�ĵ�������
						csMap.IncDestnBoxNum();
						//�������������ƶ���������ϣ����λ����Ŀ�ĵأ����ָ�ԭ�ȵĿյ�
						enPlayerMoveBlock = Map::Blank;
						enPlayerCurrentBlock = Map::BoxInDestn;
					}
					break;
				case 5:
					{
						//�������������ƶ���������ϣ����λ����Ŀ�ĵأ����ָ�ԭ�ȵ�Ŀ�ĵ�
						enPlayerMoveBlock = Map::Destn;
						enPlayerCurrentBlock = Map::BoxInDestn;
					}
					break;
				default:
					{
						return false;//���ִ�����ǰ���أ���ִ�к�������
					}
					break;
				}
			}
			break;
		}

		// Խ���ж�
		if (csPlayer.x - stMoveData.cXMove < 0 || csPlayer.x - stMoveData.cXMove >= csMap.Width() ||
			csPlayer.y - stMoveData.cYMove < 0 || csPlayer.y - stMoveData.cYMove >= csMap.Hight())
		{
			return false;
		}

		//�����ҷ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerReverseMoveBlock = csMap(csPlayer.x - stMoveData.cXMove, csPlayer.y - stMoveData.cYMove);
		if (enPlayerReverseMoveBlock == Map::Blank)
		{
			//�������״̬
			csPlayer.enPlayerStatus = Player::PlayerInBlank;
		}
		else if (enPlayerReverseMoveBlock == Map::Destn)
		{
			//�������״̬
			csPlayer.enPlayerStatus = Player::PlayerInDestn;
		}

		//�����ƶ����
		csPlayer.x -= stMoveData.cXMove;
		csPlayer.y -= stMoveData.cYMove;
		--csRecord;//�ݼ��ƶ�����

		return true;
	}

	//�����ƶ�
	bool RedoMove(void)
	{
		//��ȡβ��һ��
		Move_Data stMoveData;
		if (!csUndo.GetTail(stMoveData))
		{
			return false;
		}
		//����鿴�ɹ�����β��һ����Ԫ�أ��˴������ٴ��ж�
		csUndo.MoveTailToTail(csOperate);//ת�Ƶ��û�����������

		// Խ���ж�
		if (csPlayer.x + stMoveData.cXMove < 0 || csPlayer.x + stMoveData.cXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.cYMove < 0 || csPlayer.y + stMoveData.cYMove >= csMap.Hight())
		{
			return false;
		}

		//��õ�ǰ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.cXMove, csPlayer.y + stMoveData.cYMove);

		switch (stMoveData.ucCode)
		{
		case 0:
			{
				//�������״̬
				csPlayer.enPlayerStatus = Player::PlayerInBlank;
			}
			break;
		case 1:
			{
				//�������״̬
				csPlayer.enPlayerStatus = Player::PlayerInDestn;
			}
			break;
		default:
			{
				//Խ���ж�
				if (csPlayer.x + stMoveData.cXMove * 2 < 0 || csPlayer.x + stMoveData.cXMove * 2 >= csMap.Width() ||
					csPlayer.y + stMoveData.cYMove * 2 < 0 || csPlayer.y + stMoveData.cYMove * 2 >= csMap.Hight())
				{
					return false;
				}

				//�����Ҷ����ƶ������ϵĵ�ͼʵ��
				Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + stMoveData.cXMove * 2, csPlayer.y + stMoveData.cYMove * 2);

				switch (stMoveData.ucCode)
				{
				case 2:
					{
						//�ƶ���
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//�������״̬
						csPlayer.enPlayerStatus = Player::PlayerInBlank;
					}
					break;
				case 3:
					{
						//����Ŀ�ĵ�������
						csMap.IncDestnBoxNum();
						//�ƶ���
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//�������״̬
						csPlayer.enPlayerStatus = Player::PlayerInBlank;
					}
					break;
				case 4:
					{
						//�ݼ�Ŀ�ĵ�������
						csMap.DecDestnBoxNum();
						//�ƶ���
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//�������״̬
						csPlayer.enPlayerStatus = Player::PlayerInDestn;
					}
					break;
				case 5:
					{
						//�ƶ���
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//�������״̬
						csPlayer.enPlayerStatus = Player::PlayerInDestn;
					}
					break;
				default:
					{
						return false;//���ִ�����ǰ���أ���ִ�к�������
					}
					break;
				}
			}
			break;
		}

		//�ƶ����
		csPlayer.x += stMoveData.cXMove;
		csPlayer.y += stMoveData.cYMove;
		++csRecord;//�����ƶ�����

		return true;
	}

	size_t GetStep(void)//��ȡ����(����)
	{
		return csRecord.Current();
	}
};