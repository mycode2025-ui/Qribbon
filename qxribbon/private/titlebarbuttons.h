#pragma once
#include "qxribbon_global.h"
#include <QWidget>
#include <QToolButton>

QX_RIBBON_BEGIN_NAMESPACE

    class TitleBarButtons : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarButtons(QWidget *parent = nullptr);

    // 关键：给 RibbonBar 的布局一个明确尺寸
    QSize sizeHint() const override;

    // 同步当前窗口状态（最大/还原）
    void syncFromWindow();

protected:
    void showEvent(QShowEvent *ev) override;
    void changeEvent(QEvent *ev) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void attachToWindow();
    QWidget *tlw() const;
    void updateMaxRestoreIcon();

private slots:
    void onMinClicked();
    void onMaxClicked();
    void onCloseClicked();

private:
    QWidget *m_topLevel = nullptr;
    QToolButton *m_minBtn = nullptr;
    QToolButton *m_maxBtn = nullptr;
    QToolButton *m_closeBtn = nullptr;
};

QX_RIBBON_END_NAMESPACE
