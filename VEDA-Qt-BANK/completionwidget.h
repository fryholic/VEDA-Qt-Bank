#ifndef COMPLETIONWIDGET_H
#define COMPLETIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CheckAnimationWidget;

class CompletionWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompletionWidget(QWidget *parent = nullptr);
    
    void setupForTransaction(const QString &transactionType, double amount);
    
signals:
    void backToAccountRequested();
    
private:
    QLabel *m_completeTextLabel;
    QLabel *m_amountLabel;
    CheckAnimationWidget *m_checkAnimation;
};

#endif // COMPLETIONWIDGET_H
