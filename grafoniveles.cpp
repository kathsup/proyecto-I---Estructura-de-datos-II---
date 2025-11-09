#include "grafoniveles.h"

GrafoNiveles::GrafoNiveles(QObject *parent)
    : QObject(parent)
{
    inicializarGrafo();
}

void GrafoNiveles::inicializarGrafo()
{
    // Definir la estructura del grafo de niveles
    // Nivel 1 (Video) desbloquea Nivel 2
    niveles[1] = Nivel(1, "Video", {2});

    // Nivel 2 (Ruleta) desbloquea Nivel 3
    niveles[2] = Nivel(2, "Ruleta", {3});

    // Nivel 3 (Batalla) desbloquea Nivel 4
    niveles[3] = Nivel(3, "Batalla", {4});

    // Nivel 4 (Casa) no desbloquea nada más (nivel final)
    niveles[4] = Nivel(4, "Casa", {});

    // Niveles desbloqueados al inicio
    nivelesIniciales = {1, 2}; // Video y Ruleta ya desbloqueados

    //qDebug() << "✅ Grafo de niveles inicializado";
    //qDebug() << "   Nivel 1 (Video) → Nivel 2";
    //qDebug() << "   Nivel 2 (Ruleta) → Nivel 3";
    //qDebug() << "   Nivel 3 (Batalla) → Nivel 4";
    //qDebug() << "   Nivel 4 (Casa) → FIN";
}

bool GrafoNiveles::estaDesbloqueado(int nivelId) const
{
    // Los niveles iniciales siempre están desbloqueados
    if (nivelesIniciales.contains(nivelId)) {
        return true;
    }

    // Verificar si algún nivel completado desbloquea este nivel
    for (const Nivel& nivel : niveles) {
        if (nivel.completado && nivel.desbloquea.contains(nivelId)) {
            return true;
        }
    }

    return false;
}

void GrafoNiveles::completarNivel(int nivelId)
{
    if (!niveles.contains(nivelId)) {
        //qWarning() << "⚠️ Nivel" << nivelId << "no existe en el grafo";
        return;
    }

    if (niveles[nivelId].completado) {
       // qDebug() << "ℹ️ Nivel" << nivelId << "ya estaba completado";
        return;
    }

    // Marcar como completado
    niveles[nivelId].completado = true;
    emit nivelCompletado(nivelId);

    //qDebug() << "✅ Nivel" << nivelId << "completado!";

    // Desbloquear niveles siguientes
    QVector<int> nivelesADesbloquear = niveles[nivelId].desbloquea;
    for (int siguienteNivel : nivelesADesbloquear) {
        if (estaDesbloqueado(siguienteNivel)) {
            //qDebug() << "   🔓 Nivel" << siguienteNivel << "desbloqueado!";
            emit nivelDesbloqueado(siguienteNivel);
        }
    }
}

bool GrafoNiveles::estaCompletado(int nivelId) const
{
    if (!niveles.contains(nivelId)) {
        return false;
    }
    return niveles[nivelId].completado;
}

QVector<int> GrafoNiveles::obtenerNivelesDesbloqueados() const
{
    QVector<int> desbloqueados;

    for (const Nivel& nivel : niveles) {
        if (estaDesbloqueado(nivel.id)) {
            desbloqueados.append(nivel.id);
        }
    }

    return desbloqueados;
}

void GrafoNiveles::reiniciar()
{
    for (Nivel& nivel : niveles) {
        nivel.completado = false;
    }
   // qDebug() << "🔄 Grafo de niveles reiniciado";
}

QString GrafoNiveles::obtenerNombreNivel(int nivelId) const
{
    if (niveles.contains(nivelId)) {
        return niveles[nivelId].nombre;
    }
    return "Desconocido";
}

QVector<int> GrafoNiveles::obtenerNivelesQueDesbloquea(int nivelId) const
{
    if (niveles.contains(nivelId)) {
        return niveles[nivelId].desbloquea;
    }
    return {};
}
