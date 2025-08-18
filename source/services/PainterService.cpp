#include "PainterService.h"
#include <QDebug>

#include <QElapsedTimer>

PainterService::PainterService(QObject *parent)
    : BaseService(parent){

    QElapsedTimer timer;
    timer.start();

    int width = 1024;
    int height = 1024;

    // Circle center
    int cx = width / 2;
    int cy = height / 2;

    // Circle radius
    int radius = 100; // adjust as needed
    int r2 = radius * radius; // radius squared (avoid sqrt for speed)

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int dx = i - cx;
            int dy = j - cy;
            if (dx * dx + dy * dy <= r2) {
                markSprayed(i, j);
            }
        }
    }
    qint64 elapsedMs = timer.elapsed();

    qDebug() << elapsedMs;

    // for (auto& [id, tile] : tiles) {
    //     for(auto pixel : tile.pixels){
    //         qDebug() << pixel;
    //     }
    // }
}
