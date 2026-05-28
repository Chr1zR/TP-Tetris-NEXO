#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include "paleta.h"
#include "config.h"
#include "piezas.h"
#include "ranking.h"

#define PRESENTACION_CANT_OPCIONES 6

#define ALTO_LETRA 5
#define ALTO_NUMERO 7

/**
 * @desc Dibuja el fondo decorativo a los lados del tablero.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void fondoHudDibujar(tConfigPantalla* config);

/**
 * @desc Calcula posiciones de los recuadros del HUD.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void disposicionHudCalcular(tConfigPantalla* config);

/**
 * @desc Dibuja los bordes de las cajas del HUD.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudRecuadrosDibujar(tConfigPantalla* config);

/**
 * @desc Dibuja una palabra con fuente bitmap.
 * @param palabra const char* Texto a dibujar.
 * @param pos_x uint16_t Coordenada X.
 * @param pos_y uint16_t Coordenada Y.
 * @param color uint8_t Indice de color.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudPalabraDibujar(const char* palabra, uint16_t pos_x, uint16_t pos_y, uint8_t color, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la caja de puntaje.
 * @param score uint32_t Valor del puntaje.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudScoreDibujar(uint32_t score, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la caja de nivel.
 * @param nivel uint8_t Nivel actual.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudNivelDibujar(uint8_t nivel, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la caja de lineas limpiadas.
 * @param lineas uint16_t Cantidad de lineas.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudLineasDibujar(uint16_t lineas, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la caja de tiempo en formato MM:SS.
 * @param tiempo_segundos uint16_t Tiempo transcurrido.
 * @param escala uint8_t Factor de escala.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudTiempoDibujar(uint16_t tiempo_segundos, uint8_t escala, tConfigPantalla* config);

/**
 * @desc Dibuja la preview de la pieza siguiente.
 * @param siguiente tTetromino* Puntero al tetromino siguiente.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudNextDibujar(tTetromino* siguiente, tConfigPantalla* config);

/**
 * @desc Dibuja conteo de cada tipo de pieza usada.
 * @param conteo uint16_t[11] Array con cantidades.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudPiezasDibujar(uint16_t conteo[11], tConfigPantalla* config);

/**
 * @desc Dibuja titulo "TETRIS" y subtitulo en panel izquierdo.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void hudTituloNeonDibujar(tConfigPantalla* config);

/**
 * @desc Dibuja overlay de pausa con opciones.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param opcion_seleccionada uint8_t Indice resaltado.
 * @param confirmacion_guardado uint8_t Estado del OK de guardado.
 * @return void
 */
void cubrirPausaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t confirmacion_guardado);

/**
 * @desc Dibuja overlay de Game Over con estadisticas.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param score uint32_t Puntaje final.
 * @param lineas uint16_t Lineas totales.
 * @param nombre const char* Nombre del jugador.
 * @param opcion_seleccionada uint8_t Indice resaltado.
 * @return void
 */
void cubrirGameOverDibujar(tConfigPantalla* config, uint32_t score, uint16_t lineas, const char* nombre, uint8_t opcion_seleccionada);

/**
 * @desc Dibuja dialogo de confirmacion para salir.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void cubrirConfirmarSalidaDibujar(tConfigPantalla* config);

/**
 * @desc Dibuja la pantalla de titulo/menu principal.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param opcion_seleccionada uint8_t Indice resaltado.
 * @return void
 */
void presentarPantallaDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada);

/**
 * @desc Dibuja el menu de opciones con valores editables.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param opcion_seleccionada uint8_t Indice resaltado.
 * @param modalidad uint8_t Modo de juego.
 * @param columnas_dx uint8_t Cantidad de columnas.
 * @param velocidad double Velocidad inicial.
 * @param circular uint8_t Tablero circular.
 * @param extras uint8_t Piezas extras.
 * @param resolucion uint8_t Resolucion seleccionada.
 * @param confirmacion_guardado uint8_t Estado de confirmacion.
 * @param cheats bool Trucos habilitados.
 * @return void
 */
void menuConfiguracionDibujar(tConfigPantalla* config, uint8_t opcion_seleccionada, uint8_t modalidad, uint8_t columnas_dx, double velocidad, uint8_t circular, uint8_t extras, uint8_t resolucion, uint8_t confirmacion_guardado, bool cheats);

/**
 * @desc Dibuja overlay con lista de trucos disponibles.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void pausaCheatsDibujar(tConfigPantalla* config);

/**
 * @desc Dibuja pantalla de informacion "Acerca de".
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @return void
 */
void pantallaAcercaDeDibujar(tConfigPantalla* config);

/**
 * @desc Dibuja pantalla de carga de partida.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param hay_partida bool true si existe partida guardada.
 * @param nombre const char* Nombre del jugador guardado.
 * @param puntos uint32_t Puntaje guardado.
 * @param nivel uint16_t Nivel guardado.
 * @param lineas uint16_t Lineas guardadas.
 * @param segundos uint16_t Tiempo guardado.
 * @param mensaje_error const char* Mensaje de error si fallo.
 * @return void
 */
void pantallaCargarPartidaDibujar(tConfigPantalla* config, bool hay_partida,
                                 const char* nombre, uint32_t puntos,
                                 uint16_t nivel, uint16_t lineas, uint16_t segundos,
                                 const char* mensaje_error);

/**
 * @desc Dibuja tabla de puntajes altos.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param rank tRanking* Puntero al ranking.
 * @param confirmar_borrar uint8_t true si muestra confirmacion.
 * @return void
 */
void pantallaPuntajesDibujar(tConfigPantalla* config, tRanking* rank, uint8_t confirmar_borrar);

/**
 * @desc Dibuja pantalla de ingreso de nombre.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param buffer const char* Texto ingresado.
 * @param cursor_pos uint8_t Posicion del cursor.
 * @param cursor_visible bool true si cursor visible.
 * @return void
 */
void pantallaIngresarNombreDibujar(tConfigPantalla* config, const char* buffer, uint8_t cursor_pos, bool cursor_visible);

/**
 * @desc Dibuja un mensaje generico centrado.
 * @param config tConfigPantalla* Configuracion de pantalla.
 * @param titulo const char* Titulo del mensaje.
 * @param mensaje const char* Cuerpo del mensaje.
 * @return void
 */
void pantallaMensajeDibujar(tConfigPantalla* config, const char* titulo, const char* mensaje);

#endif // HUD_H_INCLUDED
