#include "titlebarbuttons.h"
#include <QHBoxLayout>
#include <QStyle>
#include <QApplication>
#include <QEvent>
#include <QWindow>

QX_RIBBON_BEGIN_NAMESPACE   // <--- 加上

TitleBarButtons::TitleBarButtons(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("qt_titlebar_buttons"));
    setAttribute(Qt::WA_TransparentForMouseEvents, false);

    auto lay = new QHBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    m_minBtn = new QToolButton(this);
    m_maxBtn = new QToolButton(this);
    m_closeBtn = new QToolButton(this);

    auto btnSetup = [](QToolButton *b) {
        b->setAutoRaise(true);
        b->setFocusPolicy(Qt::NoFocus);
        b->setToolButtonStyle(Qt::ToolButtonIconOnly);
        b->setFixedSize(QSize(36, 28));   // 固定尺寸：布局才有非零 sizeHint
    };

    btnSetup(m_minBtn);
    btnSetup(m_maxBtn);
    btnSetup(m_closeBtn);

    m_minBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton,  nullptr, this));
    m_closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton, nullptr, this));
    m_minBtn->setToolTip(tr("Minimize"));
    m_maxBtn->setToolTip(tr("Maximize"));
    m_closeBtn->setToolTip(tr("Close"));

    lay->addWidget(m_minBtn);
    lay->addWidget(m_maxBtn);
    lay->addWidget(m_closeBtn);

    // 关键：告诉布局“我就是个固定宽高的块”
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(m_minBtn,  &QToolButton::clicked, this, &TitleBarButtons::onMinClicked);
    connect(m_maxBtn,  &QToolButton::clicked, this, &TitleBarButtons::onMaxClicked);
    connect(m_closeBtn,&QToolButton::clicked, this, &TitleBarButtons::onCloseClicked);

    // 初始同步一次
    syncFromWindow();
}

// 新增：给出稳定的 sizeHint（按你固定的按钮尺寸计算）
QSize TitleBarButtons::sizeHint() const
{
    if (!m_minBtn) return QWidget::sizeHint();
    const QSize s = m_minBtn->size();
    // 3 个按钮 + layout spacing(这里为0)
    return QSize(s.width() * 3, s.height());
}

void TitleBarButtons::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev);
    attachToWindow();
    syncFromWindow();
}

void TitleBarButtons::changeEvent(QEvent *ev)
{
    QWidget::changeEvent(ev);
    if (ev->type() == QEvent::StyleChange
        || ev->type() == QEvent::PaletteChange
        || ev->type() == QEvent::FontChange) {   // 可选：字体变也刷新一下
        m_minBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton,  nullptr, this));
        m_closeBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton, nullptr, this));
        updateMaxRestoreIcon();
    }
}

bool TitleBarButtons::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == m_topLevel) {
        switch (ev->type()) {
        case QEvent::WindowStateChange:
        case QEvent::ActivationChange:
        case QEvent::Resize:
        case QEvent::Show:
        case QEvent::Hide:
            syncFromWindow();
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void TitleBarButtons::attachToWindow()
{
    QWidget *w = tlw();
    if (w == m_topLevel)
        return;

    if (m_topLevel)
        m_topLevel->removeEventFilter(this);

    m_topLevel = w;
    if (m_topLevel)
        m_topLevel->installEventFilter(this);
}

QWidget *TitleBarButtons::tlw() const
{
    return window();
}

void TitleBarButtons::syncFromWindow()
{
    updateMaxRestoreIcon();
    QWidget *w = tlw();
    if (!w) return;

    const bool canMin = w->windowFlags().testFlag(Qt::WindowMinimizeButtonHint);
    const bool canMax = w->windowFlags().testFlag(Qt::WindowMaximizeButtonHint);

    m_minBtn->setEnabled(canMin);
    m_maxBtn->setEnabled(canMax);
}

void TitleBarButtons::updateMaxRestoreIcon()
{
    QWidget *w = tlw();
    if (!w) return;

    const bool maximized = w->isMaximized();
    const bool full = w->isFullScreen();

    if (maximized || full) {
        m_maxBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton, nullptr, this));
        m_maxBtn->setToolTip(tr("Restore"));
    } else {
        m_maxBtn->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton, nullptr, this));
        m_maxBtn->setToolTip(tr("Maximize"));
    }
}

void TitleBarButtons::onMinClicked()
{
    if (auto *w = tlw())
        w->showMinimized();
}

void TitleBarButtons::onMaxClicked()
{
    if (auto *w = tlw()) {
        if (w->isFullScreen() || w->isMaximized())
            w->showNormal();
        else
            w->showMaximized();
        updateMaxRestoreIcon();
    }
}

void TitleBarButtons::onCloseClicked()
{
    if (auto *w = tlw())
        w->close();
}

QX_RIBBON_END_NAMESPACE   // <--- 加上
