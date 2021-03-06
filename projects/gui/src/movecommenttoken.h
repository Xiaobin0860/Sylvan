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

#ifndef MOVECOMMENTTOKEN_H
#define MOVECOMMENTTOKEN_H

#include "pgntoken.h"

/*!
 * \brief A comment token for a move in a PGN game
 */
class MoveCommentToken : public PgnToken
{
public:
    /*! Creates a new MoveCommentToken with text \a text at \a ply. */
    MoveCommentToken(int ply, const QString& text);

    // Inherited from PgnToken
    virtual QString toString() const;

    /*!
         * Sets the comment's text to \a text.
         * Changes to the text document have to be made separately.
         */
    void setValue(const QString& text);

protected:
    // Inherited from PgnToken
    virtual void vInsert(QTextCursor& cursor);

private:
    int m_ply;
    QString m_text;
};

#endif // MOVECOMMENTTOKEN_H
