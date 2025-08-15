#include <Arduino.h>
#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>
#include <vector>
#include <memory>

// Inclure les composants UI nécessaires
// PlatformIO trouvera les bibliothèques dans le dossier 'lib'
#include "UIComponent.h"
#include "UICheckbox.h"

#define BACKLIGHT_PIN 32

// Objets globaux pour l'écran et la police
TFT_eSPI tft = TFT_eSPI();
U8g2_for_TFT_eSPI u8f;

// Vecteur pour gérer tous nos composants UI
std::vector<std::unique_ptr<UIComponent>> components;

// Variables pour la gestion simple du toucher
static bool wasTouched = false;
uint16_t touchX, touchY;

void setBacklight(uint8_t intensity) {
    analogWrite(BACKLIGHT_PIN, intensity);
}
void setup() {
    // 1. Initialisation
    Serial.begin(115200);
    Serial.println("Exemple de test pour UICheckbox");

    pinMode(BACKLIGHT_PIN, OUTPUT);
    setBacklight(255); // Allumer le rétroéclairage

    tft.begin();
    tft.setRotation(3); // Ajustez la rotation selon votre écran (0-3)

    tft.fillScreen(TFT_BLACK);uint16_t calData[5] = { 255, 3626, 216, 3663, 1 };
    tft.setTouch(calData);

    // Calibrage de l'écran tactile (si nécessaire)
    // uint16_t calData[5] = { ... };
    // tft.setTouch(calData);

    u8f.begin(tft); // Initialise le helper U8g2

    // 2. Définition des styles
    // Style pour une case à cocher standard
    UICheckboxStyle defaultStyle;
    defaultStyle.labelStyle.font = u8g2_font_profont17_tr;
    defaultStyle.labelStyle.textColor = TFT_WHITE;
    defaultStyle.labelStyle.bgColor = TFT_BLACK;
    defaultStyle.labelStyle.disabledTextColor = TFT_DARKGREY;
    defaultStyle.labelStyle.disabledBgColor = TFT_BLACK;
    defaultStyle.boxColor = TFT_CYAN;
    defaultStyle.checkColor = TFT_GREEN;
    defaultStyle.disabledBoxColor = TFT_DARKGREY;
    defaultStyle.disabledCheckColor = TFT_DARKGREY;
    defaultStyle.boxSize = 24;
    defaultStyle.spacing = 12;

    // 3. Création des composants UICheckbox
    
    // Checkbox 1: Activée et cochée par défaut
    auto checkbox1 = new UICheckbox(u8f, {20, 30, 280, 40}, "Activer le WiFi", defaultStyle, true);
    checkbox1->onToggle([](bool state) {
        Serial.print("WiFi ");
        Serial.println(state ? "active." : "desactive.");
    });
    components.emplace_back(checkbox1);

    // Checkbox 2: Activée et non cochée par défaut
    auto checkbox2 = new UICheckbox(u8f, {20, 80, 280, 40}, "Envoyer les rapports", defaultStyle, false);
    checkbox2->onToggle([](bool state) {
        Serial.print("Rapports ");
        Serial.println(state ? "actives." : "desactives.");
    });
    components.emplace_back(checkbox2);

    // Checkbox 3: Désactivée par défaut pour montrer le style "disabled"
    auto checkbox3 = new UICheckbox(u8f, {20, 130, 280, 40}, "Mode Administrateur", defaultStyle, false);
    checkbox3->setEnabled(false); // On la désactive
    checkbox3->onToggle([](bool state) {
        // Ce message ne devrait jamais s'afficher car la case est désactivée
        Serial.println("Mode admin change !");
    });
    components.emplace_back(checkbox3);

    // Forcer un premier dessin de tous les composants
    tft.fillScreen(TFT_BLACK); // Nettoyer l'écran
    for (const auto& comp : components) {
        comp->setDirty(true); // Marquer pour le dessin
        comp->draw(tft);
    }
}

void loop() {
    // 4. Gestion du toucher et mise à jour de l'UI
    bool isTouched = tft.getTouch(&touchX, &touchY);

    // Gérer l'appui (press)
    if (isTouched && !wasTouched) {
        for (const auto& comp : components) {
            comp->handlePress(tft, touchX, touchY);
        }
    }

    // Gérer le relâchement (release)
    if (!isTouched && wasTouched) {
        for (const auto& comp : components) {
            comp->handleRelease(tft, touchX, touchY);
        }
    }

    wasTouched = isTouched;

    // 5. Redessiner les composants qui ont changé
    for (const auto& comp : components) {
        if (comp->isDirty()) {
            comp->draw(tft);
        }
    }

    delay(20); // Petite pause pour éviter de surcharger le CPU
}