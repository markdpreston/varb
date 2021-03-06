/*
 *  VarB - genomic analysis software
 *
 *  software.markdpreston.com/varb
 *
 *  (c) Mark D Preston, H Magnus Manske 2011-
 *
 *  When using this software, commercially or academically, please
 *  get in contact and reference:
 *
 *  Mark D Preston, H Magnus Manske, et al.
 *  VarB: A Variation Browsing tool for polymorphisms derived from
 *  next-generation sequence data
 *  In submission
 *
 *
 *  This software was written, in large part, at the London School of
 *  Hygiene and Tropical Medicine, UK and the Sanger Institute
 *  Cambridge, UK.
 *
 *
 *  This file is part of VarB.
 *
 *  VarB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  VarB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with VarB.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QScrollArea>
#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include "searchresult.h"

typedef struct {
    QString chromosome;
    long    from;       //  1-based
    long    to;         //  1-based
    long    readDepth;
    long    quality;
    bool    gene;
    bool    insertion;
    bool    deletion;
    bool    synonymous;
    bool    nonSynonymous;
} Range;

class TrackContainer;

class Track : public QObject {
    Q_OBJECT
public:
    explicit Track(QObject *parent = 0);
    ~Track();
    virtual void clear();
    virtual void paint(QPainter &painter);
    virtual void paintLabel(QPainter &painter);
    virtual void setRange (Range range);
    virtual void recalculateLayout();
    virtual QString whatsthis() { return "track"; }
    virtual void runSearch(QString query, QList <SearchResult*> &results) { Q_UNUSED(query) Q_UNUSED(results) }
    virtual void mouseEvent(QMouseEvent *event,QString region,bool doubleclick);
    virtual void changeChromosome() { }
    virtual void changePosition() { }
    virtual void changeFilter() { }
    virtual void changeGroups() { }

    long height, top, width, label_offset;
    Range range;
    QRect rLabel, rContent;
    QString title;

protected:
    virtual void doTheDrag(QMouseEvent *event);

    TrackContainer *area;
    QString label;
    bool forceRedraw;
};

#endif // TRACK_H
