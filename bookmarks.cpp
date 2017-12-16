#include <QtWidgets>

#include "bookmarks.h"
#include "xbeltree.h"

Bookmarks::Bookmarks(QWidget *parent):QDialog(parent) {
    xbelTree = new XbelTree;
    //setCentralWidget(xbelTree);
    menuBar = new QMenuBar();
    createMenus();

    setWindowTitle(tr("DOM Bookmarks"));
    const QSize availableSize = QApplication::desktop()->availableGeometry(this).size();
    resize(availableSize.width() / 2, availableSize.height() / 3);
}

void Bookmarks::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
                                         QDir::currentPath(),
                                         tr("XBEL Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }
}

void Bookmarks::saveAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save Bookmark File"),
                                         QDir::currentPath(),
                                         tr("XBEL Files (*.xbel *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }
}

void Bookmarks::about()
{
   QMessageBox::about(this, tr("About DOM Bookmarks"),
                      tr("The <b>DOM Bookmarks</b> example demonstrates how to "
                         "use Qt's DOM classes to read and write XML "
                         "documents."));
}

void Bookmarks::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &Bookmarks::open);
    openAct->setShortcuts(QKeySequence::Open);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &Bookmarks::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcuts(QKeySequence::Quit);

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &Bookmarks::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QCoreApplication::quit);
}
