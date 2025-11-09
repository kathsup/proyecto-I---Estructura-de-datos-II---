#ifndef GRAFONIVELES_H
#define GRAFONIVELES_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QString>

struct Nivel {
    int id;
    QString nombre;
    bool completado;
    QVector<int> desbloquea; // IDs de niveles que desbloquea este nivel

    Nivel(int i = 0, QString n = "", QVector<int> d = {})
        : id(i), nombre(n), completado(false), desbloquea(d) {}
};

class GrafoNiveles : public QObject
{
    Q_OBJECT

public:
    explicit GrafoNiveles(QObject *parent = nullptr);

    // Métodos principales
    bool estaDesbloqueado(int nivelId) const;
    void completarNivel(int nivelId);
    bool estaCompletado(int nivelId) const;
    QVector<int> obtenerNivelesDesbloqueados() const;
    void reiniciar();

    // Información del grafo
    QString obtenerNombreNivel(int nivelId) const;
    QVector<int> obtenerNivelesQueDesbloquea(int nivelId) const;

signals:
    void nivelDesbloqueado(int nivelId);
    void nivelCompletado(int nivelId);

private:
    QMap<int, Nivel> niveles;
    QVector<int> nivelesIniciales; // Niveles desbloqueados desde el inicio

    void inicializarGrafo();
};


#endif // GRAFONIVELES_H
