#include <QApplication>
#include <QStringList>
#include <QWindow>

#include "app/window.hpp"
#include "app/action.hpp"

int main(int argc, char *argv[])
{
    QApplication dash(argc, argv);

    dash.setOrganizationName("openDsh");
    dash.setApplicationName("dash");
    dash.installEventFilter(ActionEventFilter::get_instance());

    QSize size = dash.primaryScreen()->size();
    QPoint pos = dash.primaryScreen()->geometry().topLeft();
    bool fullscreen = true;

    QSettings settings;
    DASH_LOG(info) << "loaded config: " << settings.fileName().toStdString();

    QStringList args = dash.arguments();
    if (args.size() > 2) {
        size = QSize(args.at(1).toInt(), args.at(2).toInt());
        if (args.size() > 4)
            pos = QPoint(args.at(3).toInt(), args.at(4).toInt());
        fullscreen = false;
    }
    else {
        settings.beginGroup("Window");
        if (settings.contains("size")) {
            size = settings.value("size").toSize();
            if (settings.contains("pos"))
                pos = settings.value("pos").toPoint();
            fullscreen = false;
        }
    }

    dash.processEvents();

    MainWindow window(QRect(pos, size));
    window.setWindowIcon(QIcon(":/icons/android_auto_color.svg"));
    window.setWindowFlags(Qt::FramelessWindowHint);
    if (fullscreen)
        window.setWindowState(Qt::WindowFullScreen);

    window.show();

    return dash.exec();
}
