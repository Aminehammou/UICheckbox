#include "UICheckbox.h"
//#include "ThemeManager.h" // Pour accéder à la couleur de fond globale

namespace { // Constantes pour la lisibilité
    constexpr int kCheckmarkInset = 2; // Marge intérieure pour la coche
    constexpr int kCheckmarkBorder = 4; // Marge totale (2*inset)
}

UICheckbox::UICheckbox(U8g2_for_TFT_eSPI& u8f, const UIRect& rect, const String& text, const UICheckboxStyle& style, bool initialState)
    : UITextComponent(u8f, rect, text), _style(style), _checked(initialState), _onToggleCallback(nullptr) {}

void UICheckbox::drawInternal(TFT_eSPI& tft, bool force) {
    // 1. Choisir les couleurs en fonction de l'état (activé/désactivé)
    const uint16_t boxColor = enabled ? _style.boxColor : _style.disabledBoxColor;
    const uint16_t checkColor = enabled ? _style.checkColor : _style.disabledCheckColor;
    const uint16_t textColor = enabled ? _style.labelStyle.textColor : _style.labelStyle.disabledTextColor;
    const uint16_t bgColor = enabled ? _style.labelStyle.bgColor : _style.labelStyle.disabledBgColor;

    // 2. Dessiner le fond du composant
    tft.fillRect(rect.x, rect.y, rect.w, rect.h, bgColor);

    // 3. Dessiner la boîte
    const int boxY = rect.y + (rect.h - _style.boxSize) / 2; // Centrer verticalement la boîte
    tft.drawRect(rect.x, boxY, _style.boxSize, _style.boxSize, boxColor);

    // 4. Dessiner la coche si la case est cochée
    if (_checked) {
        const int cx = rect.x + kCheckmarkInset;
        const int cy = boxY + kCheckmarkInset;
        const int csize = _style.boxSize - kCheckmarkBorder;

        // Points pour une coche de style '✓'
        const int p1x = cx + csize / 5;      // Point de départ de la branche courte
        const int p1y = cy + csize / 2;
        const int p2x = cx + csize * 2 / 5;  // Sommet (point le plus bas)
        const int p2y = cy + csize * 3 / 4;
        const int p3x = cx + csize * 4 / 5;  // Point d'arrivée de la branche longue
        const int p3y = cy + csize / 4;

        tft.drawLine(p1x, p1y, p2x, p2y, checkColor); // Branche courte
        tft.drawLine(p2x, p2y, p3x, p3y, checkColor); // Branche longue
        // Pour rendre la coche plus épaisse, ajoutez ces lignes :
        tft.drawLine(p1x + 1, p1y, p2x + 1, p2y, checkColor);
        tft.drawLine(p2x + 1, p2y, p3x + 1, p3y, checkColor);
    }

    // 5. Dessiner le texte du label avec U8g2
    _u8f.setFontMode(1);
    _u8f.setFontDirection(0);
    _u8f.setForegroundColor(textColor);
    _u8f.setBackgroundColor(bgColor);
    _u8f.setFont(_style.labelStyle.font);

    // Calculer la position du texte
    // L'alignement est ML_DATUM (Milieu-Gauche)
    const int textX = rect.x + _style.boxSize + _style.spacing;
    // Pour l'alignement vertical au milieu, on utilise la même technique que pour les autres composants
    int16_t textH = _u8f.getFontAscent() - _u8f.getFontDescent();
    const int textY = rect.y + (rect.h + textH) / 2; // Baseline pour centrer verticalement

    _u8f.setCursor(textX, textY);
    _u8f.print(_text);
}

void UICheckbox::handleRelease(TFT_eSPI& tft, int tx, int ty) {
    if (enabled && contains(tx, ty)) {
        // Inverser l'état
        _checked = !_checked;

        // Appeler le callback s'il a été défini
        if (_onToggleCallback) {
            _onToggleCallback(_checked);
        }

        setDirty(true); // Marquer pour un redessin au prochain cycle
    }
}

void UICheckbox::setChecked(bool checked, bool triggerCallback) {
    if (_checked != checked) {
        _checked = checked;
        if (triggerCallback && _onToggleCallback) {
            _onToggleCallback(_checked);
        }
        setDirty(true);
    }
}

bool UICheckbox::isChecked() const {
    return _checked;
}

void UICheckbox::onToggle(std::function<void(bool)> callback) {
    _onToggleCallback = callback;
}