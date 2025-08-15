#ifndef UILABELSTYLE_H
#define UILABELSTYLE_H

#include "U8g2_for_TFT_eSPI.h" // Pour les polices U8g2

#include <TFT_eSPI.h> // Pour les définitions de couleurs comme TFT_WHITE

/**
 * @struct UILabelStyle
 * @brief Définit l'apparence visuelle d'un composant UILabel.
 * 
 * Cette structure regroupe toutes les propriétés de style d'une étiquette,
 * permettant de découpler l'apparence de la logique du composant.
 */
struct UILabelStyle {
    // Couleurs pour l'état normal
    uint16_t textColor = TFT_WHITE;
    uint16_t bgColor   = TFT_BLACK;

    // Couleurs pour l'état désactivé
    uint16_t disabledTextColor = TFT_DARKGREY;
    uint16_t disabledBgColor   = TFT_BLACK;

    const uint8_t* font = u8g2_font_ncenB08_tf; // Police U8g2 par défaut
    uint8_t  textAlign = MC_DATUM; // Par défaut : Milieu-Centre. Voir TFT_eSPI.h pour les valeurs (ML_DATUM, MC_DATUM, etc.)
    uint8_t  padding   = 5; // Marge intérieure en pixels

    uint16_t borderColor = TFT_WHITE; // Couleur de la bordure
    uint8_t  borderWidth = 0;       // Épaisseur de la bordure en pixels
    uint8_t  borderRadius = 0;      // Rayon de courbure de la bordure en pixels
};

#endif // UILABELSTYLE_H