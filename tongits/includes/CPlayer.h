#ifndef CPLAYER_H
#define CPLAYER_H

#include "CDeck.h"

class CPlayer
{
public:
	CPlayer()	{ mMyTurn = mDidGetNewCard = mDidChow = mHasSets = mCanCall = false; }
	~CPlayer()	{}

	void SetTurn(bool pMyTurn)			{ mMyTurn = pMyTurn;	}
	void SetWon(bool pWon)				{ mWon = pWon;			}
	void SetIdx(int pIdx)				{ mIdx = pIdx;			}
	bool IsItMyTurn()					{ return mMyTurn;		}
	bool Won()							{ return mWon;			}
	int  GetIdx()						{ return mIdx;			}

	void SetMano()						{ mDidGetNewCard = mDidChow = true; }

	//moves
	bool GetNewCard(CDeck &pDeck);
	bool Chow(CDeck &pDeck);
	bool Dump(CDeck &pDeck);
	bool Sapaw(CDeck &pDeck, int pRgn);
	bool Call(CDeck &pDeck);
	bool Show(CDeck &pDeck, int pRgn);
	//AI moves
	bool AutoSapaw(CDeck &pDeck);
	void StartTurn(CDeck &pDeck);

	bool DidChow()						{ return mDidChow;       }
	bool DidGetNewCard()				{ return mDidGetNewCard; }

	void SetHasSets(bool pHasSets)		{ mHasSets = pHasSets;   }
	bool HasSets()						{ return mHasSets;       }
	void SetCanCall(bool pCanCall)		{ mCanCall = pCanCall;   }
	bool CanCall()						{ return mCanCall;       }

protected:
	int		mIdx;
	bool	mMyTurn;
	bool	mDidGetNewCard;
	bool	mDidChow;
	bool	mWon;

	bool	mHasSets;
	bool	mCanCall;
};

#endif //CPLAYER_H
