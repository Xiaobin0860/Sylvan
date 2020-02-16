/*
    This file is part of Sylvan.
    Copyright (C) 2008-2018 Cute Chess authors
    Copyright (C) 2019 Wilbert Lee

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Sylvan.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QVariant>
#include <QStringList>
#include "westernboard.h"
#include "westernzobrist.h"
#include "boardtransition.h"

namespace Chess {

WesternBoard::WesternBoard(WesternZobrist* zobrist)
    : Board(zobrist),
      m_arwidth(0),
      m_sign(1),
      m_plyOffset(0),
      m_reversibleMoveCount(0),
      m_zobrist(zobrist)
{
    setPieceType(Pawn, tr("pawn"), "P");
    setPieceType(Horse, tr("horse"), "N");
    setPieceType(Elephant, tr("elephant"), "B");
    setPieceType(Guard, tr("guard"), "A");
    setPieceType(Rook, tr("rook"), "R");
    setPieceType(Cannon, tr("cannon"), "C");
    setPieceType(King, tr("king"), "K");
}

int WesternBoard::width() const
{
    return 9;
}

int WesternBoard::height() const
{
    return 10;
}

bool WesternBoard::kingsCountAssertion(int redKings, int blackKings) const
{
    return redKings == 1 && blackKings == 1;
}

void WesternBoard::vInitialize()
{
    m_arwidth = width() + 2;

    m_kingSquare[Side::Red] = 0;
    m_kingSquare[Side::Black] = 0;

    m_BPawnOffsets.resize(3);
    m_BPawnOffsets[0] = m_arwidth;
    m_BPawnOffsets[1] = -1;
    m_BPawnOffsets[2] = 1;

    m_RPawnOffsets.resize(3);
    m_RPawnOffsets[0] = -m_arwidth;
    m_RPawnOffsets[1] = -1;
    m_RPawnOffsets[2] = 1;

    m_HorseLegOffsets.resize(8);
    m_HorseLegOffsets[0] = -m_arwidth;
    m_HorseLegOffsets[1] = -m_arwidth;
    m_HorseLegOffsets[2] = -1;
    m_HorseLegOffsets[3] = +1;
    m_HorseLegOffsets[4] = -1;
    m_HorseLegOffsets[5] = +1;
    m_HorseLegOffsets[6] = +m_arwidth;
    m_HorseLegOffsets[7] = +m_arwidth;

    m_HorseOffsets.resize(8);
    m_HorseOffsets[0] = -2 * m_arwidth - 1;
    m_HorseOffsets[1] = -2 * m_arwidth + 1;
    m_HorseOffsets[2] = -m_arwidth - 2;
    m_HorseOffsets[3] = -m_arwidth + 2;
    m_HorseOffsets[4] = m_arwidth - 2;
    m_HorseOffsets[5] = m_arwidth + 2;
    m_HorseOffsets[6] = 2 * m_arwidth - 1;
    m_HorseOffsets[7] = 2 * m_arwidth + 1;

    m_HorseCheckLegOffsets.resize(8);
    m_HorseCheckLegOffsets[0] = -m_arwidth-1;
    m_HorseCheckLegOffsets[1] = -m_arwidth+1;
    m_HorseCheckLegOffsets[2] = -m_arwidth -1;
    m_HorseCheckLegOffsets[3] = -m_arwidth +1;
    m_HorseCheckLegOffsets[4] = m_arwidth -1;
    m_HorseCheckLegOffsets[5] = m_arwidth +1;
    m_HorseCheckLegOffsets[6] = +m_arwidth-1;
    m_HorseCheckLegOffsets[7] = +m_arwidth+1;

    m_ElephantOffsets.resize(4);
    m_ElephantOffsets[0] = -2 * m_arwidth - 2;
    m_ElephantOffsets[1] = -2 * m_arwidth + 2;
    m_ElephantOffsets[2] = 2 * m_arwidth - 2;
    m_ElephantOffsets[3] = 2 * m_arwidth + 2;

    m_ElephantEyeOffsets.resize(4);
    m_ElephantEyeOffsets[0] = -m_arwidth - 1;
    m_ElephantEyeOffsets[1] = -m_arwidth + 1;
    m_ElephantEyeOffsets[2] = m_arwidth - 1;
    m_ElephantEyeOffsets[3] = m_arwidth + 1;

    m_RookOffsets.resize(4);
    m_RookOffsets[0] = -m_arwidth;
    m_RookOffsets[1] = -1;
    m_RookOffsets[2] = 1;
    m_RookOffsets[3] = m_arwidth;

    m_GuardOffsets.resize(4);
    m_GuardOffsets[0] = -m_arwidth - 1;
    m_GuardOffsets[1] = -m_arwidth + 1;
    m_GuardOffsets[2] = m_arwidth - 1;
    m_GuardOffsets[3] = m_arwidth + 1;

    strnumCn.resize(10);
    strnumCn[0] = "〇";
    strnumCn[1] = "一";
    strnumCn[2] = "二";
    strnumCn[3] = "三";
    strnumCn[4] = "四";
    strnumCn[5] = "五";
    strnumCn[6] = "六";
    strnumCn[7] = "七";
    strnumCn[8] = "八";
    strnumCn[9] = "九";

    strnumEn.resize(10);
    strnumEn[0] = "０";
    strnumEn[1] = "１";
    strnumEn[2] = "２";
    strnumEn[3] = "３";
    strnumEn[4] = "４";
    strnumEn[5] = "５";
    strnumEn[6] = "６";
    strnumEn[7] = "７";
    strnumEn[8] = "８";
    strnumEn[9] = "９";

    strnumName.resize(16);
    strnumName[0] = " ";
    strnumName[1] = "兵";
    strnumName[2] = "相";
    strnumName[3] = "仕";
    strnumName[4] = "炮";
    strnumName[5] = "马";
    strnumName[6] = "车";
    strnumName[7] = "帅";
    strnumName[8]  = "卒";
    strnumName[9]  = "象";
    strnumName[10] = "士";
    strnumName[11] = "炮";
    strnumName[12] = "马";
    strnumName[13] = "车";
    strnumName[14] = "将";
    strnumName[15] = " ";
}

int WesternBoard::captureType(const Move& move) const
{
    return Board::captureType(move);
}

Move WesternBoard::moveFromStandardString(const QString& str)
{
    QVarLengthArray<Move> moves;
    generateMoves(moves);

    for (int i = 0; i < moves.size(); i++)
    {
        QString cn = standardMoveString(moves[i]);

        if (str == cn)
        {
            if (vIsLegalMove(moves[i]))
            {
                return moves[i];
            }
        }
    }

    return Move();
}

QString WesternBoard::lanMoveString(const Move& move)
{
    return Board::lanMoveString(move);
}


QString WesternBoard::standardMoveString(const Move& move)
{
    QString str;
    int source = move.sourceSquare();
    int target = move.targetSquare();
    Piece piece = pieceAt(source);
    Square FromSquare = chessSquare(source);
    Square ToSqure = chessSquare(target);

    Side side = sideToMove();

    int fx = FromSquare.file();
    int tx = ToSqure.file();
    int fy = FromSquare.rank();
    int ty = ToSqure.rank();

    int chessType = piece.type();
    bool isQH = false;
    QString stQH = "";

    if (chessType != Elephant || chessType != Guard || chessType != King)
    {
        int targetSquare = source;
        while(true){
            targetSquare -= 11;
            if (!isValidSquare(chessSquare(targetSquare)))
                break;
            Piece mpiece = pieceAt(targetSquare);
            if (mpiece.side() == side) {
                if (mpiece.type() == chessType) {
                    if (side == Side::Red) {
                        stQH = "后";
                    }
                    else {
                        stQH = "前";
                    }
                    isQH = true;
                    goto QH_BRANCH;
                }
            }
        }
        targetSquare = source;
        while (true) {
            targetSquare += 11;
            if (!isValidSquare(chessSquare(targetSquare)))
                break;
            Piece mpiece = pieceAt(targetSquare);
            if (mpiece.side() == side) {
                if (mpiece.type() == chessType) {
                    if (side == Side::Red) {
                        stQH = "前";
                    }
                    else {
                        stQH = "后";
                    }
                    isQH = true;
                    goto QH_BRANCH;
                }
            }
        }
    }


QH_BRANCH:
    if (isQH == true) {
        str = stQH;
        if (side == Side::Red) {
            str += strnumName[chessType];
            if (ty == fy) {
                str += "平";
                str += strnumCn[10 - (tx + 1)];
            }
            else {
                if (target < source) {
                    str += "进";
                }
                else {
                    str += "退";
                }
                if (chessType == Elephant || chessType == Guard || chessType == Horse) {
                    str += strnumCn[10 - (tx + 1)];
                }
                else {
                    str += strnumCn[abs(fy - ty)];
                }
            }
        }
        else {
            str += strnumName[chessType + 7];
            if (ty == fy) {
                str += "平";
                str += strnumEn[(tx + 1)];
            }
            else {
                if (target < source) {
                    str += "退";
                }
                else {
                    str += "进";
                }
                if (chessType == Elephant || chessType == Guard || chessType == Horse) {
                    str += strnumEn[(tx + 1)];
                }
                else {
                    str += strnumEn[abs(fy - ty)];
                }
            }
        }
    }
    else {
        if (side == Side::Red) {
            if (ty == fy) {
                str = strnumName[chessType];
                str += strnumCn[10 - (fx + 1)];
                str += "平";
                str += strnumCn[10 - (tx + 1)];
            }
            else {
                str = strnumName[chessType];
                str += strnumCn[10 - (fx + 1)];
                if (target < source) {
                    str += "进";
                }
                else {
                    str += "退";
                }
                if (chessType == Elephant || chessType == Guard || chessType == Horse) {
                    str += strnumCn[10 - (tx + 1)];
                }
                else {
                    str += strnumCn[abs(fy-ty)];
                }
            }
        }
        else {
            if (ty == fy) {
                str = strnumName[chessType + 7];
                str += strnumEn[(fx + 1)];
                str += "平";
                str += strnumEn[(tx + 1)];
            }
            else {
                str = strnumName[chessType + 7];
                str += strnumEn[(fx + 1)];
                if (target < source) {
                    str += "退";
                }
                else {
                    str += "进";
                }
                if (chessType == Elephant || chessType == Guard || chessType == Horse) {
                    str += strnumEn[(tx + 1)];
                }
                else {
                    str += strnumEn[abs(fy - ty)];
                }
            }
        }
    }

    return str;
}

Move WesternBoard::moveFromLanString(const QString& str)
{
    Move move(Board::moveFromEnglishString(str));
    return move;
}

QString WesternBoard::vFenIncludeString(FenNotation notation) const
{
    Q_UNUSED(notation);
    return "";
}

QString WesternBoard::vFenString(FenNotation notation) const
{
    QString fen  =vFenIncludeString(notation);

    // Reversible halfmove count
    fen += "- - ";
    fen += QString::number(m_reversibleMoveCount);

    // Full move number
    fen += ' ';
    fen += QString::number((m_history.size() + m_plyOffset) / 2 + 1);

    return fen;
}

bool WesternBoard::vSetFenString(const QStringList& fen)
{
    if (fen.size() < 2)
        return false;
    QStringList::const_iterator token = fen.begin();

    // Find the king squares
    int kingCount[2] = {0, 0};
    for (int sq = 0; sq < arraySize(); sq++)
    {
        Piece tmp = pieceAt(sq);
        if (tmp.type() == King)
        {
            m_kingSquare[tmp.side()] = sq;
            kingCount[tmp.side()]++;
        }
    }
    if (!kingsCountAssertion(kingCount[Side::Red],
                             kingCount[Side::Black]))
        return false;

    // Short non-standard format without castling and ep fields?
    bool isShortFormat = false;
    if (fen.size() < 3)
        token->toInt(&isShortFormat);

    if (!isShortFormat)
        ++token;

    // En-passant square
    Side side(sideToMove());
    m_sign = (side == Side::Red) ? 1 : -1;

    if (!isShortFormat)
        ++token;

    // Reversible halfmove count
    if (token != fen.end())
    {
        bool ok;
        int tmp = token->toInt(&ok);
        if (!ok || tmp < 0)
            return false;
        m_reversibleMoveCount = tmp;
        ++token;
    }
    else
        m_reversibleMoveCount = 0;

    // Read the full move number and calculate m_plyOffset
    if (token != fen.end())
    {
        bool ok;
        int tmp = token->toInt(&ok);
        if (!ok || tmp < 1)
            return false;
        m_plyOffset = 2 * (tmp - 1);
    }
    else
        m_plyOffset = 0;

    if (m_sign != 1)
        m_plyOffset++;

    m_history.clear();
    return true;
}

void WesternBoard::vMakeMove(const Move& move, BoardTransition* transition)
{
    Side side = sideToMove();
    int source = move.sourceSquare();
    int target = move.targetSquare();
    Piece capture = pieceAt(target);
    int pieceType = pieceAt(source).type();

    bool clearSource = true;
    bool isReversible = true;

    Q_ASSERT(target != 0);

    MoveData md = { capture, m_reversibleMoveCount };

    if (source == target)
        clearSource = 0;

    if (pieceType == King)
        m_kingSquare[side] = target;

    if (captureType(move) != Piece::NoPiece)
        isReversible = false;

    if (transition != nullptr)
        if (source != 0)
            transition->addMove(chessSquare(source),
                                chessSquare(target));

    setSquare(target, Piece(side, pieceType));

    if (clearSource)
        setSquare(source, Piece::NoPiece);

    if (isReversible)
        m_reversibleMoveCount++;
    else
        m_reversibleMoveCount = 0;

    m_history.append(md);
    m_sign *= -1;
}

void WesternBoard::vUndoMove(const Move& move)
{
    const MoveData& md = m_history.last();
    int source = move.sourceSquare();
    int target = move.targetSquare();

    m_sign *= -1;
    Side side = sideToMove();

    m_reversibleMoveCount = md.reversibleMoveCount;

    if (target == m_kingSquare[side])
        m_kingSquare[side] = source;

    setSquare(source, pieceAt(target));
    setSquare(target, md.capture);
    m_history.pop_back();
}

void WesternBoard::generateMovesForPiece(QVarLengthArray<Move>& moves,
                                         int pieceType,
                                         int sourceSquare) const
{		
    switch (pieceType)
    {
    case Pawn:
    {
        Piece piece = pieceAt(sourceSquare);
        if (piece.side() == Side::Red) {
            Side opSide = sideToMove().opposite();
            for (int i = 0; i < m_RPawnOffsets.size(); i++)
            {
                int targetSquare = sourceSquare + m_RPawnOffsets[i];
                if (!isValidSquare(chessSquare(targetSquare)))
                    continue;
                if (sourceSquare > 75)
                    if (i != 0)  continue;

                Piece capture = pieceAt(targetSquare);
                if (capture.isEmpty() || capture.side() == opSide)
                    moves.append(Move(sourceSquare, targetSquare));
            }
        }
        else
        {
            Side opSide = sideToMove().opposite();
            for (int i = 0; i < m_BPawnOffsets.size(); i++)
            {
                int targetSquare = sourceSquare + m_BPawnOffsets[i];
                if (!isValidSquare(chessSquare(targetSquare)))
                    continue;
                if (sourceSquare < 78)
                    if (i != 0)  continue;

                Piece capture = pieceAt(targetSquare);
                if (capture.isEmpty() || capture.side() == opSide)
                    moves.append(Move(sourceSquare, targetSquare));
            }
        }
    }
        break;
    case King:
    {
        Side opSide = sideToMove().opposite();
        for (int i = 0; i < m_RookOffsets.size(); i++)
        {
            int targetSquare = sourceSquare + m_RookOffsets[i];
            if (!isValidSquare(chessSquare(targetSquare)))
                continue;

            if (!isInFort(chessSquare(targetSquare)))
                continue;

            Piece capture = pieceAt(targetSquare);
            if (capture.isEmpty() || capture.side() == opSide)
                moves.append(Move(sourceSquare, targetSquare));
        }
    }
        break;
    case Guard:
    {
        Side opSide = sideToMove().opposite();
        for (int i = 0; i < m_GuardOffsets.size(); i++)
        {
            int targetSquare = sourceSquare + m_GuardOffsets[i];
            if (!isValidSquare(chessSquare(targetSquare)))
                continue;

            if (!isInFort(chessSquare(targetSquare)))
                continue;

            Piece capture = pieceAt(targetSquare);
            if (capture.isEmpty() || capture.side() == opSide)
                moves.append(Move(sourceSquare, targetSquare));
        }
    }
        break;
    case Rook:
    {
        Side side = sideToMove();
        for (int i = 0; i < m_RookOffsets.size(); i++)
        {
            int offset = m_RookOffsets[i];
            int targetSquare = sourceSquare + offset;
            Piece capture;
            while (!(capture = pieceAt(targetSquare)).isWall()
                   && capture.side() != side)
            {
                moves.append(Move(sourceSquare, targetSquare));
                if (!capture.isEmpty())
                    break;
                targetSquare += offset;
            }
        }
    }
        break;
    case Cannon:
    {
        Side side = sideToMove();
        for (int i = 0; i < m_RookOffsets.size(); i++)
        {
            int offset = m_RookOffsets[i];
            int targetSquare = sourceSquare + offset;
            Piece capture;
            while (!(capture = pieceAt(targetSquare)).isWall() && capture.isEmpty())
            {
                moves.append(Move(sourceSquare, targetSquare));
                if (!capture.isEmpty())
                    break;
                targetSquare += offset;
            }
            if (!capture.isEmpty())
            {
                while (true)
                {
                    targetSquare += offset;
                    capture = pieceAt(targetSquare);
                    if (capture.isEmpty())
                        continue;
                    if (capture.isWall())
                        break;
                    if (capture.side() != side)
                    {
                        moves.append(Move(sourceSquare, targetSquare));
                        break;
                    }
                }
            }
        }
        break;
    }
    case Horse:
    {
        Side opSide = sideToMove().opposite();
        for (int i = 0; i < m_HorseOffsets.size(); i++)
        {
            int targetSquare = sourceSquare + m_HorseOffsets[i];
            if (!isValidSquare(chessSquare(targetSquare)))
                continue;
            int leg = sourceSquare + m_HorseLegOffsets[i];
            if (!pieceAt(leg).isEmpty())
                continue;
            Piece capture = pieceAt(targetSquare);
            if (capture.isEmpty() || capture.side() == opSide)
                moves.append(Move(sourceSquare, targetSquare));
        }
    }
        break;
    case Elephant:
    {
        Side opSide = sideToMove().opposite();
        for (int i = 0; i < m_ElephantOffsets.size(); i++)
        {
            int targetSquare = sourceSquare + m_ElephantOffsets[i];
            if (!isValidSquare(chessSquare(targetSquare)))
                continue;
            int leg = sourceSquare + m_ElephantEyeOffsets[i];
            if (!pieceAt(leg).isEmpty())
                continue;

            Piece capture = pieceAt(targetSquare);
            if (capture.isEmpty() || capture.side() == opSide)
            {
                if (sourceSquare > 78 && targetSquare < 78)
                    continue;
                if (sourceSquare < 78 && targetSquare > 78)
                    continue;
                moves.append(Move(sourceSquare, targetSquare));
            }
        }
    }
        break;
    }
}

bool WesternBoard::inCheck(Side side) const
{
    Side opSide = side.opposite();
    Piece piece;

    int ksquare = m_kingSquare[side];

    for (int i = 0; i < m_RookOffsets.size(); i++)
    {
        int offset = m_RookOffsets[i];
        int targetSquare = ksquare + offset;

        int count = 0;
        while (true)
        {
            if (!isValidSquare(chessSquare(targetSquare)))
                break;

            piece = pieceAt(targetSquare);
            if (!piece.isEmpty())
            {
                count++;
                if (count == 1)
                {
                    if (piece.side() == opSide)
                    {
                        if (piece.type() == Rook || piece.type() == King)
                            return true;
                        else if (piece.type() == Pawn)
                        {
                            if (abs(targetSquare - ksquare) == 1)
                                return true;
                            if (side == Side::Red)
                            {
                                if (targetSquare == ksquare - 11)
                                    return true;
                            }
                            else
                            {
                                if (targetSquare == ksquare + 11)
                                    return true;
                            }
                        }
                    }
                }
                else if (count == 2)
                {
                    if (piece.side() == opSide)
                        if (piece.type() == Cannon)
                            return true;
                    break;
                }
            }
            targetSquare += offset;
        }
    }

    // Knight, archbishop, chancellor attacks
    for (int i = 0; i < m_HorseOffsets.size(); i++)
    {
        piece = pieceAt(ksquare + m_HorseOffsets[i]);
        if (piece.side() == opSide && piece.type() == Horse)
        {
            Piece leg = pieceAt(ksquare + m_HorseCheckLegOffsets[i]);

            if (leg.isEmpty())
                return true;
        }
    }

    return false;
}

bool WesternBoard::isLegalPosition()
{
    Side side = sideToMove().opposite();
    if (inCheck(side))
        return false;

    if (m_history.isEmpty())
        return true;

    return true;
}

bool WesternBoard::vIsLegalMove(const Move& move)
{
    Q_ASSERT(!move.isNull());
    return Board::vIsLegalMove(move);
}

int WesternBoard::kingSquare(Side side) const
{
    Q_ASSERT(!side.isNull());
    return m_kingSquare[side];
}

int WesternBoard::reversibleMoveCount() const
{
    return m_reversibleMoveCount;
}

Result WesternBoard::result()
{
    QString str;

    // Checkmate/Stalemate
    if (!canMove())
    {
        Side winner = sideToMove().opposite();
        str = tr("%1 Win!").arg(winner.toString());
        return Result(Result::Win, winner, str);
    }

    // Insufficient mating material
    int material = 0;

    for (int i = 0; i < arraySize(); i++)
    {
        const Piece& piece = pieceAt(i);
        if (!piece.isValid())
            continue;

        switch (piece.type())
        {
        case King:
            break;
        case Guard:
            break;
        case Elephant:
            material++;
            break;
        default:
            material += 2;
            break;
        }
    }

    if (material <= 0)
    {
        str = tr("Both sides have no attacks, judged draw!");
        return Result(Result::Draw, Side::NoSide, str);
    }

    if (m_reversibleMoveCount >= 120)
    {
        str = tr("60 step did not eat, judged draw!");
        return Result(Result::Draw, Side::NoSide, str);
    }

    if (repeatCount() >= 2)
    {
        str = tr("Cycle 3 steps, judged draw!");
        return Result(Result::Draw, Side::NoSide, str);
    }

    return Result();
}

} // namespace Chess
