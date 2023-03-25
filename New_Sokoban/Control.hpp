#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "List.hpp"


//�����߼�
class Game_Control
{
public:
	struct Move_Data
	{
		long lXMove, lYMove;//XY�ƶ�����
		long lCode;//������
	};
private:
	Map &csMap;
	Player &csPlayer;
	List<Move_Data> csOperate;//������¼
	List<Move_Data> csUndo;//������¼
public:
	Game_Control(Map &_csMap, Player &_csPlayer) :
		csMap(_csMap),csPlayer(_csPlayer)
	{}
	~Game_Control(void) = default;

	size_t GetStep(void)
	{
		return csOperate.GetNodeNum();
	}

	Map &GetMap(void)
	{
		return csMap;
	}

	Player &GetPlayer(void)
	{
		return csPlayer;
	}

	bool IsWin(void)
	{
		return csMap.DestnBoxNum() == csMap.AllBoxNum();
	}

	//�ƶ�
	bool MovePlayer(long lXMove, long lYMove)
	{
		// Խ���ж�
		if (csPlayer.x + lXMove < 0 || csPlayer.x + lXMove >= csMap.Width() ||
			csPlayer.y + lYMove < 0 || csPlayer.y + lYMove >= csMap.Hight())
		{
			return false;
		}

		//��õ�ǰ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + lXMove, csPlayer.y + lYMove);

		if (enPlayerMoveBlock == Map::Wall)//���ǰ����ǽ��
		{
			return false;
		}
		else if (enPlayerMoveBlock == Map::Blank)//���ǰ���ǿյ�
		{
			//�������״̬
			csPlayer.enPlayer = Player::PlayerInBlank;
			//��������
			csOperate.InsertTail({lXMove,lYMove,0});
		}
		else if (enPlayerMoveBlock == Map::Destn)//���ǰ����Ŀ�ĵ�
		{
			//�������״̬
			csPlayer.enPlayer = Player::PlayerInDestn;
			//��������
			csOperate.InsertTail({lXMove,lYMove,1});
		}
		else//�������
		{
			//Խ���ж�
			if (csPlayer.x + lXMove * 2 < 0 || csPlayer.x + lXMove * 2 >= csMap.Width() ||
				csPlayer.y + lYMove * 2 < 0 || csPlayer.y + lYMove * 2 >= csMap.Hight())
			{
				return false;
			}

			//�����Ҷ����ƶ������ϵĵ�ͼʵ��
			Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + lXMove * 2, csPlayer.y + lYMove * 2);

			if (enPlayerMoveBlock == Map::BoxInBlank)//���ǰ�����ڿյ��ϵ�����
			{
				if (enPlayerDoubleMoveBlock == Map::Blank)//����ǰ���ǿյ�
				{
					//�ƶ���
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//�������״̬
					csPlayer.enPlayer = Player::PlayerInBlank;
					//��������
					csOperate.InsertTail({lXMove,lYMove,2});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//����ǰ����Ŀ�ĵ�
				{
					//����Ŀ�ĵ�������
					++csMap.DestnBoxNum();
					//�ƶ���
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//�������״̬
					csPlayer.enPlayer = Player::PlayerInBlank;
					//��������
					csOperate.InsertTail({lXMove,lYMove,3});
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
					--csMap.DestnBoxNum();
					//�ƶ���
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//�������״̬
					csPlayer.enPlayer = Player::PlayerInDestn;
					//��������
					csOperate.InsertTail({lXMove,lYMove,4});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//����ǰ����Ŀ�ĵ�
				{
					//�ƶ���
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//�������״̬
					csPlayer.enPlayer = Player::PlayerInDestn;
					//��������
					csOperate.InsertTail({lXMove,lYMove,5});
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
		csPlayer.x += lXMove;
		csPlayer.y += lYMove;
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


		// Խ���ж�
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}
		
		//�������ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.lXMove, csPlayer.y + stMoveData.lYMove);
		//�����ҵ�ǰλ���ϵĵ�ͼʵ��
		Map::Block &enPlayerCurrentBlock = csMap(csPlayer.x, csPlayer.y);

		switch (stMoveData.lCode)
		{
		case 0:
		case 1:
			break;//�޵�ͼ������ɶ������
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
				--csMap.DestnBoxNum();
				//�������������ƶ���������ϣ����λ���ǿյأ����ָ�ԭ�ȵ�Ŀ�ĵ�
				enPlayerMoveBlock = Map::Destn;
				enPlayerCurrentBlock = Map::BoxInBlank;
			}
			break;
		case 4:
			{
				//����Ŀ�ĵ�������
				++csMap.DestnBoxNum();
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

		// Խ���ж�
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}

		//�����ҷ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerReverseMoveBlock = csMap(csPlayer.x - stMoveData.lXMove, csPlayer.y - stMoveData.lYMove);
		if (enPlayerReverseMoveBlock == Map::Blank)
		{
			//�������״̬
			csPlayer.enPlayer = Player::PlayerInBlank;
		}
		else if (enPlayerReverseMoveBlock == Map::Destn)
		{
			//�������״̬
			csPlayer.enPlayer = Player::PlayerInDestn;
		}
		//�����ƶ����
		csPlayer.x -= stMoveData.lXMove;
		csPlayer.y -= stMoveData.lYMove;

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
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}

		//��õ�ǰ����ƶ������ϵĵ�ͼʵ��
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.lXMove, csPlayer.y + stMoveData.lYMove);

		switch (stMoveData.lCode)
		{
		case 0:
			{
				//�������״̬
				csPlayer.enPlayer = Player::PlayerInBlank;
			}
			break;
		case 1:
			{
				//�������״̬
				csPlayer.enPlayer = Player::PlayerInDestn;
			}
			break;
		default:
			{
				//Խ���ж�
				if (csPlayer.x + stMoveData.lXMove * 2 < 0 || csPlayer.x + stMoveData.lXMove * 2 >= csMap.Width() ||
					csPlayer.y + stMoveData.lYMove * 2 < 0 || csPlayer.y + stMoveData.lYMove * 2 >= csMap.Hight())
				{
					return false;
				}

				//�����Ҷ����ƶ������ϵĵ�ͼʵ��
				Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + stMoveData.lXMove * 2, csPlayer.y + stMoveData.lYMove * 2);

				switch (stMoveData.lCode)
				{
				case 2:
					{
						//�ƶ���
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//�������״̬
						csPlayer.enPlayer = Player::PlayerInBlank;
					}
					break;
				case 3:
					{
						//����Ŀ�ĵ�������
						++csMap.DestnBoxNum();
						//�ƶ���
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//�������״̬
						csPlayer.enPlayer = Player::PlayerInBlank;
					}
					break;
				case 4:
					{
						//�ݼ�Ŀ�ĵ�������
						--csMap.DestnBoxNum();
						//�ƶ���
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//�������״̬
						csPlayer.enPlayer = Player::PlayerInDestn;
					}
					break;
				case 5:
					{
						//�ƶ���
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//�������״̬
						csPlayer.enPlayer = Player::PlayerInDestn;
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
		csPlayer.x += stMoveData.lXMove;
		csPlayer.y += stMoveData.lYMove;
		return true;
	}
};


#include "Draw.hpp"