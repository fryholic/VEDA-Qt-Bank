#include "checkanimationwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

CheckAnimationWidget::CheckAnimationWidget(QWidget *parent)
    : QWidget(parent)
    , m_progress(0.0)
    , m_transactionType("deposit")
{
    // 기본 크기 설정
    setFixedSize(120, 120);
    
    // 애니메이션 설정
    m_animation = new QPropertyAnimation(this, "progress", this);
    m_animation->setDuration(800);
    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 기본 색상 설정
    updateColors();
}

CheckAnimationWidget::~CheckAnimationWidget()
{
    delete m_animation;
}

void CheckAnimationWidget::startAnimation()
{
    m_progress = 0.0;
    m_animation->start();
    update();
}

void CheckAnimationWidget::setTransactionType(const QString &type)
{
    m_transactionType = type;
    updateColors();
    update();
}

void CheckAnimationWidget::setProgress(qreal progress)
{
    if (m_progress != progress) {
        m_progress = progress;
        update();
    }
}

void CheckAnimationWidget::updateColors()
{
    if (m_transactionType == "deposit") {
        m_circleColor = QColor(46, 204, 113); // 초록색
        m_checkColor = Qt::white;
    } else if (m_transactionType == "withdraw") {
        m_circleColor = QColor(230, 126, 34); // 주황색
        m_checkColor = Qt::white;
    } else { // transfer
        m_circleColor = QColor(52, 152, 219); // 파란색
        m_checkColor = Qt::white;
    }
}

void CheckAnimationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int width = this->width();
    int height = this->height();
    int size = qMin(width, height);
    
    // 원 그리기
    int circleThickness = size / 12;
    int circleRadius = (size - circleThickness) / 2;
    QRectF circleRect(width/2 - circleRadius, height/2 - circleRadius, 
                      circleRadius * 2, circleRadius * 2);
    
    // 배경 원 (회색)
    painter.setPen(QPen(QColor(200, 200, 200), circleThickness));
    painter.drawEllipse(circleRect);
    
    // 진행 원 (색상)
    painter.setPen(QPen(m_circleColor, circleThickness));
    painter.drawArc(circleRect, 90 * 16, -m_progress * 360 * 16);
    
    // 체크 마크 그리기 (애니메이션 완료 시)
    if (m_progress > 0.7) {
        qreal checkProgress = (m_progress - 0.7) / 0.3; // 0.7에서 1.0까지 정규화
        
        QPainterPath path;
        int checkSize = size / 2;
        
        // 체크 마크의 시작점
        QPoint startPoint(width/2 - checkSize/3, height/2 + checkSize/6);
        
        // 체크 마크의 중간점
        QPoint midPoint(width/2 - checkSize/12, height/2 + checkSize/3);
        
        // 체크 마크의 끝점
        QPoint endPoint(width/2 + checkSize/2, height/2 - checkSize/4);
        
        // 첫 번째 선 (왼쪽 아래 방향)
        if (checkProgress <= 0.5) {
            qreal firstLineProgress = checkProgress * 2.0;
            QPoint currentMidPoint = startPoint + (midPoint - startPoint) * firstLineProgress;
            path.moveTo(startPoint);
            path.lineTo(currentMidPoint);
        } else {
            // 두 번째 선 (오른쪽 위 방향)
            qreal secondLineProgress = (checkProgress - 0.5) * 2.0;
            path.moveTo(startPoint);
            path.lineTo(midPoint);
            path.lineTo(midPoint + (endPoint - midPoint) * secondLineProgress);
        }
        
        // 체크 마크에 테두리 효과 추가
        int checkThickness = circleThickness * 1.2;
        
        // 테두리 그리기 (검은색 또는 어두운 색상)
        painter.setPen(QPen(QColor(50, 50, 50, 180), checkThickness + 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
        
        // 체크 마크 그리기
        painter.setPen(QPen(m_checkColor, checkThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(path);
    }
}
