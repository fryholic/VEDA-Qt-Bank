#ifndef CHECKANIMATIONWIDGET_H
#define CHECKANIMATIONWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>

class CheckAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress)
    
public:
    explicit CheckAnimationWidget(QWidget *parent = nullptr);
    ~CheckAnimationWidget();
    
    void startAnimation();
    void setTransactionType(const QString &type);
    
    qreal progress() const { return m_progress; }
    void setProgress(qreal progress);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    
private:
    qreal m_progress;
    QPropertyAnimation *m_animation;
    QString m_transactionType;
    QColor m_circleColor;
    QColor m_checkColor;
    
    void updateColors();
};

#endif // CHECKANIMATIONWIDGET_H
