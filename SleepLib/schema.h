#ifndef SCHEMA_H
#define SCHEMA_H

#include <QColor>
#include <QHash>
#include <QVariant>
#include <QString>
#include "machine_common.h"

namespace GraphFlags {
const quint32 Shadow=1;
const quint32 Foobar=2;
const quint32 XTicker=4;
const quint32 YTicker=8;
const quint32 XGrid=16;
const quint32 YGrid=32;
}

namespace schema {

enum Function {
    NONE=0, AVG, WAVG, MIN, MAX, SUM, CNT, P90, CPH, SPH, HOURS, SET
};

enum ChanType {
    DATA=0, SETTING
};

enum DataType {
    DEFAULT=0, INTEGER, BOOL, DOUBLE, STRING, RICHTEXT, DATE, TIME, DATETIME
};
enum ScopeType {
    GLOBAL=0, MACHINE, DAY, SESSION
};
class Channel;
extern Channel EmptyChannel;

// this is really a channel definition.
class Channel
{
public:
    Channel() { m_id=0; }
    Channel(int id, ChanType type, ScopeType scope, QString name, QString description, QString label, QString unit,DataType datatype=DEFAULT, QColor=Qt::black, int link=0);
    void addColor(Function f, QColor color) { m_colors[f]=color; }
    void addOption(int i, QString option) { m_options[i]=option; }

    const int & id() { return m_id; }
    const ChanType & type() { return m_type; }
    const QString & name() { return m_name; }
    const QString & description() { return m_description; }
    const QString & label() { return m_label; }
    QColor & defaultColor() { return m_defaultcolor; }
    void setDefaultColor(QColor color) { m_defaultcolor=color; }
    QHash<int,QString> m_options;
    QHash<Function,QColor> m_colors;
    QList<Channel *> m_links;              // better versions of this data type
    bool isNull();
protected:
    int m_id;
    ChanType m_type;
    ScopeType m_scope;
    QString m_name;
    QString m_description;
    QString m_label;
    QString m_unit;
    DataType m_datatype;
    QColor m_defaultcolor;

    int m_link;
};

class ChannelList
{
public:
    ChannelList();
    virtual ~ChannelList();
    bool Load(QString filename);
    bool Save(QString filename);
    Channel & operator[](int i) {
        if (channels.contains(i))
            return *channels[i];
        else
            return EmptyChannel;
    }
    Channel & operator[](QString name) {
        if (names.contains(name))
            return *names[name];
        else
            return EmptyChannel;
    }

    QHash<int,Channel *> channels;
    QHash<QString,Channel *> names;
    QHash<QString,QHash<QString,Channel *> > groups;
    QString m_doctype;
};
extern ChannelList channel;

enum LayerType {
    UnspecifiedLayer, Waveform, Flag, Overlay, Group
};

class Layer
{
public:
    Layer(ChannelID code, QColor colour, QString label=QString());
    virtual ~Layer();
    Layer *addLayer(Layer *layer);// { m_layers.push_back(layer); return layer; }
    void setMin(EventDataType min) { m_min=min; m_hasmin=true; }
    void setMax(EventDataType max) { m_max=max; m_hasmax=true; }
    EventDataType min() { return m_min; }
    EventDataType max() { return m_max; }
    bool visible() { return m_visible; }
    void setVisible(bool b) { m_visible=b; }
protected:
    LayerType m_type;
    ChannelID m_code;
    QColor m_colour;
    QString m_label;
    EventDataType m_min;
    EventDataType m_max;
    bool m_hasmin;
    bool m_hasmax;
    bool m_visible;
    QVector<Layer *> m_layers;
};

class WaveformLayer: public Layer
{
public:
    WaveformLayer(ChannelID code, QColor colour, float min=0, float max=0);
    virtual ~WaveformLayer();
};

enum FlagVisual { Bar, Span, Dot };
class OverlayLayer: public Layer
{
public:
    OverlayLayer(ChannelID code, QColor colour, FlagVisual visual=Bar);
    virtual ~OverlayLayer();
protected:
    FlagVisual m_visual;
};
class GroupLayer: public Layer // Effectively an empty Layer container
{
public:
    GroupLayer();
    virtual ~GroupLayer();
};
class FlagGroupLayer: public GroupLayer
{
public:
    FlagGroupLayer();
    virtual ~FlagGroupLayer();
};
class FlagLayer: public Layer
{
public:
    FlagLayer(ChannelID code, QColor colour, FlagVisual visual=Bar);
    virtual ~FlagLayer();
protected:
    FlagVisual m_visual;
};
class Graph
{
public:
    Graph(QString name,quint32 flags=GraphFlags::XTicker | GraphFlags::YTicker | GraphFlags::XGrid);
    Layer *addLayer(Layer *layer) { m_layers.push_back(layer); return layer; }
    int height() { if (m_visible) return m_height; else return 0;}
    void setHeight(int h) { m_height=h; }
    bool visible() { return m_visible; }
    void setVisible(bool b) { m_visible=b; }
protected:
    QString m_name;
    int m_height;
    QVector<Layer *> m_layers;
    bool m_visible;
};
class GraphGroup
{
public:
    GraphGroup(QString name);
    GraphGroup();
    Graph *addGraph(Graph *graph) { m_graphs.push_back(graph); return graph; }
protected:
    QVector<Graph *>m_graphs;
};

void init();

} // namespace

#endif // SCHEMA_H
