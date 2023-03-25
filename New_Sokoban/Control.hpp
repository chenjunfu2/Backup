#pragma once
#include "Map.hpp"
#include "Player.hpp"
#include "List.hpp"


//控制逻辑
class Game_Control
{
public:
	struct Move_Data
	{
		long lXMove, lYMove;//XY移动方向
		long lCode;//操作码
	};
private:
	Map &csMap;
	Player &csPlayer;
	List<Move_Data> csOperate;//操作记录
	List<Move_Data> csUndo;//撤销记录
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

	//移动
	bool MovePlayer(long lXMove, long lYMove)
	{
		// 越界判断
		if (csPlayer.x + lXMove < 0 || csPlayer.x + lXMove >= csMap.Width() ||
			csPlayer.y + lYMove < 0 || csPlayer.y + lYMove >= csMap.Hight())
		{
			return false;
		}

		//获得当前玩家移动方向上的地图实体
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + lXMove, csPlayer.y + lYMove);

		if (enPlayerMoveBlock == Map::Wall)//玩家前面是墙壁
		{
			return false;
		}
		else if (enPlayerMoveBlock == Map::Blank)//玩家前面是空地
		{
			//设置玩家状态
			csPlayer.enPlayer = Player::PlayerInBlank;
			//加入链表
			csOperate.InsertTail({lXMove,lYMove,0});
		}
		else if (enPlayerMoveBlock == Map::Destn)//玩家前面是目的地
		{
			//设置玩家状态
			csPlayer.enPlayer = Player::PlayerInDestn;
			//加入链表
			csOperate.InsertTail({lXMove,lYMove,1});
		}
		else//其它情况
		{
			//越界判断
			if (csPlayer.x + lXMove * 2 < 0 || csPlayer.x + lXMove * 2 >= csMap.Width() ||
				csPlayer.y + lYMove * 2 < 0 || csPlayer.y + lYMove * 2 >= csMap.Hight())
			{
				return false;
			}

			//获得玩家二倍移动方向上的地图实体
			Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + lXMove * 2, csPlayer.y + lYMove * 2);

			if (enPlayerMoveBlock == Map::BoxInBlank)//玩家前面是在空地上的箱子
			{
				if (enPlayerDoubleMoveBlock == Map::Blank)//箱子前面是空地
				{
					//推动它
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//设置玩家状态
					csPlayer.enPlayer = Player::PlayerInBlank;
					//加入链表
					csOperate.InsertTail({lXMove,lYMove,2});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//箱子前面是目的地
				{
					//递增目的地箱子数
					++csMap.DestnBoxNum();
					//推动它
					enPlayerMoveBlock = Map::Blank;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//设置玩家状态
					csPlayer.enPlayer = Player::PlayerInBlank;
					//加入链表
					csOperate.InsertTail({lXMove,lYMove,3});
				}
				else//未检测的项：箱子前面是：箱子、在目的地上的箱子、墙壁，直接返回
				{
					return false;
				}
			}
			else if (enPlayerMoveBlock == Map::BoxInDestn)//玩家前面是在目的地上的箱子
			{
				if (enPlayerDoubleMoveBlock == Map::Blank)//箱子前面是空地
				{
					//递减目的地箱子数
					--csMap.DestnBoxNum();
					//推动它
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInBlank;
					//设置玩家状态
					csPlayer.enPlayer = Player::PlayerInDestn;
					//加入链表
					csOperate.InsertTail({lXMove,lYMove,4});
				}
				else if (enPlayerDoubleMoveBlock == Map::Destn)//箱子前面是目的地
				{
					//推动它
					enPlayerMoveBlock = Map::Destn;
					enPlayerDoubleMoveBlock = Map::BoxInDestn;
					//设置玩家状态
					csPlayer.enPlayer = Player::PlayerInDestn;
					//加入链表
					csOperate.InsertTail({lXMove,lYMove,5});
				}
				else//未检测的项：箱子前面是：箱子、在目的地上的箱子、墙壁，直接返回
				{
					return false;
				}
			}
			else//意外情况，直接返回
			{
				return false;
			}
		}

		//移动玩家
		csPlayer.x += lXMove;
		csPlayer.y += lYMove;
		return true;
	}

	//撤销移动
	bool UndoMove(void)
	{
		//获取尾部一项
		Move_Data stMoveData;
		if (!csOperate.GetTail(stMoveData))
		{
			return false;
		}
		//上面查看成功代表尾部一定有元素，此处无需再次判断
		csOperate.MoveTailToTail(csUndo);//转移到撤销链表中


		// 越界判断
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}
		
		//获得玩家移动方向上的地图实体
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.lXMove, csPlayer.y + stMoveData.lYMove);
		//获得玩家当前位置上的地图实体
		Map::Block &enPlayerCurrentBlock = csMap(csPlayer.x, csPlayer.y);

		switch (stMoveData.lCode)
		{
		case 0:
		case 1:
			break;//无地图交互，啥都不做
		case 2:
			{
				//拉动它（箱子移动到玩家身上，玩家位置是空地）并恢复原先的空地
				enPlayerMoveBlock = Map::Blank;
				enPlayerCurrentBlock = Map::BoxInBlank;
			}
			break;
		case 3:
			{
				//递减目的地箱子数
				--csMap.DestnBoxNum();
				//拉动它（箱子移动到玩家身上，玩家位置是空地）并恢复原先的目的地
				enPlayerMoveBlock = Map::Destn;
				enPlayerCurrentBlock = Map::BoxInBlank;
			}
			break;
		case 4:
			{
				//递增目的地箱子数
				++csMap.DestnBoxNum();
				//拉动它（箱子移动到玩家身上，玩家位置是目的地）并恢复原先的空地
				enPlayerMoveBlock = Map::Blank;
				enPlayerCurrentBlock = Map::BoxInDestn;
			}
			break;
		case 5:
			{
				//拉动它（箱子移动到玩家身上，玩家位置是目的地）并恢复原先的目的地
				enPlayerMoveBlock = Map::Destn;
				enPlayerCurrentBlock = Map::BoxInDestn;
			}
			break;
		default:
			{
				return false;//出现错误提前返回，不执行后面内容
			}
			break;
		}

		// 越界判断
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}

		//获得玩家反向移动方向上的地图实体
		Map::Block &enPlayerReverseMoveBlock = csMap(csPlayer.x - stMoveData.lXMove, csPlayer.y - stMoveData.lYMove);
		if (enPlayerReverseMoveBlock == Map::Blank)
		{
			//设置玩家状态
			csPlayer.enPlayer = Player::PlayerInBlank;
		}
		else if (enPlayerReverseMoveBlock == Map::Destn)
		{
			//设置玩家状态
			csPlayer.enPlayer = Player::PlayerInDestn;
		}
		//反向移动玩家
		csPlayer.x -= stMoveData.lXMove;
		csPlayer.y -= stMoveData.lYMove;

		return true;
	}

	//重做移动
	bool RedoMove(void)
	{
		//获取尾部一项
		Move_Data stMoveData;
		if (!csUndo.GetTail(stMoveData))
		{
			return false;
		}
		//上面查看成功代表尾部一定有元素，此处无需再次判断
		csUndo.MoveTailToTail(csOperate);//转移到用户操作链表中

		// 越界判断
		if (csPlayer.x + stMoveData.lXMove < 0 || csPlayer.x + stMoveData.lXMove >= csMap.Width() ||
			csPlayer.y + stMoveData.lYMove < 0 || csPlayer.y + stMoveData.lYMove >= csMap.Hight())
		{
			return false;
		}

		//获得当前玩家移动方向上的地图实体
		Map::Block &enPlayerMoveBlock = csMap(csPlayer.x + stMoveData.lXMove, csPlayer.y + stMoveData.lYMove);

		switch (stMoveData.lCode)
		{
		case 0:
			{
				//设置玩家状态
				csPlayer.enPlayer = Player::PlayerInBlank;
			}
			break;
		case 1:
			{
				//设置玩家状态
				csPlayer.enPlayer = Player::PlayerInDestn;
			}
			break;
		default:
			{
				//越界判断
				if (csPlayer.x + stMoveData.lXMove * 2 < 0 || csPlayer.x + stMoveData.lXMove * 2 >= csMap.Width() ||
					csPlayer.y + stMoveData.lYMove * 2 < 0 || csPlayer.y + stMoveData.lYMove * 2 >= csMap.Hight())
				{
					return false;
				}

				//获得玩家二倍移动方向上的地图实体
				Map::Block &enPlayerDoubleMoveBlock = csMap(csPlayer.x + stMoveData.lXMove * 2, csPlayer.y + stMoveData.lYMove * 2);

				switch (stMoveData.lCode)
				{
				case 2:
					{
						//推动它
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//设置玩家状态
						csPlayer.enPlayer = Player::PlayerInBlank;
					}
					break;
				case 3:
					{
						//递增目的地箱子数
						++csMap.DestnBoxNum();
						//推动它
						enPlayerMoveBlock = Map::Blank;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//设置玩家状态
						csPlayer.enPlayer = Player::PlayerInBlank;
					}
					break;
				case 4:
					{
						//递减目的地箱子数
						--csMap.DestnBoxNum();
						//推动它
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInBlank;
						//设置玩家状态
						csPlayer.enPlayer = Player::PlayerInDestn;
					}
					break;
				case 5:
					{
						//推动它
						enPlayerMoveBlock = Map::Destn;
						enPlayerDoubleMoveBlock = Map::BoxInDestn;
						//设置玩家状态
						csPlayer.enPlayer = Player::PlayerInDestn;
					}
					break;
				default:
					{
						return false;//出现错误提前返回，不执行后面内容
					}
					break;
				}
			}
			break;
		}

		//移动玩家
		csPlayer.x += stMoveData.lXMove;
		csPlayer.y += stMoveData.lYMove;
		return true;
	}
};


#include "Draw.hpp"