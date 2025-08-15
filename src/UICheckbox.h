#ifndef UICHECKBOX_H
#define UICHECKBOX_H

#include "UITextComponent.h" // Hérite de UITextComponent pour la gestion du texte
#include "UIRect.h"
#include "UILabelStyle.h"
#include "U8g2_for_TFT_eSPI.h"
#include <functional>

class TFT_eSPI; // Forward declaration

/**
 * @brief Structure de style pour la case à cocher.
 */
struct UICheckboxStyle {
    UILabelStyle labelStyle;        // Style pour le label texte
    uint16_t boxColor;          // Couleur de la boîte
    uint16_t checkColor;        // Couleur de la coche
    uint16_t disabledBoxColor;  // Couleurs quand le composant est désactivé
    uint16_t disabledCheckColor;
    int      boxSize;           // Taille en pixels du carré de la checkbox
    int      spacing;           // Espace entre la case et le texte
};

class UICheckbox : public UITextComponent {
public:
    UICheckbox(U8g2_for_TFT_eSPI& u8f, const UIRect& rect, const String& text, const UICheckboxStyle& style, bool initialState = false);

    void handleRelease(TFT_eSPI& tft, int tx, int ty) override;

    void setChecked(bool checked, bool triggerCallback = false);
    bool isChecked() const;
    void onToggle(std::function<void(bool)> callback);

private:
    void drawInternal(TFT_eSPI& tft, bool force) override;
    UICheckboxStyle _style;
    bool _checked;
    std::function<void(bool)> _onToggleCallback;
};

#endif // UICHECKBOX_H