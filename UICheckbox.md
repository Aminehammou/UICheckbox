# Documentation de la classe `UICheckbox`

## 1. Introduction

La classe `UICheckbox` est un composant d'interface utilisateur qui permet à l'utilisateur de faire un choix binaire (coché ou non coché). Elle se compose d'une boîte carrée et d'un libellé textuel.

Elle hérite de `UITextComponent`, ce qui lui donne des propriétés de base comme un texte, une position et des dimensions, et lui permet d'être gérée par un `UIScreen`.

## 2. Constructeur

Pour créer une case à cocher, on utilise le constructeur suivant :

```cpp
UICheckbox(
    TFT_eSPI& tft,
    const UIRect& rect,
    const String& text,
    const UICheckboxStyle& style,
    bool initialState = false 
);
```

**Paramètres :**

* `tft` (TFT_eSPI&) : Une référence à l'objet écran.
* `x`, `y`, `w`, `h` (int) : Coordonnées et dimensions de la zone cliquable totale du composant.
* `text` (const String&) : Le texte du libellé à afficher à côté de la case.
* `style` (const UICheckboxStyle&) : Une structure définissant l'apparence de la case et du texte.
* `initialState` (bool) : L'état initial de la case (cochée ou non). Par défaut, `false`.

## 3. Style (`UICheckboxStyle`)

L'apparence de la case à cocher est entièrement contrôlée par la structure `UICheckboxStyle`.

```cpp
struct UICheckboxStyle {
    UILabelStyle labelStyle;        // Style pour le label (couleur, police, fond).
    uint16_t boxColor;              // Couleur de la boîte.
    uint16_t checkColor;            // Couleur de la coche (le '✓').
    uint16_t disabledBoxColor;      // Couleur de la boîte quand le composant est désactivé.
    uint16_t disabledCheckColor;    // Couleur de la coche quand désactivé.
    int      boxSize;               // Taille en pixels du carré.
    int      spacing;               // Espace en pixels entre la boîte et le texte.
};
```

## 4. Méthodes Principales

* `void setChecked(bool checked, bool triggerCallback = false)` : Permet de changer l'état de la case par programme. Si `triggerCallback` est `true`, le callback `onToggle` sera appelé.
* `bool isChecked() const` : Retourne l'état actuel de la case (`true` si cochée).
* `void onToggle(std::function<void(bool)> callback)` : Définit la fonction à appeler lorsque l'état de la case change suite à une interaction de l'utilisateur. La fonction lambda reçoit le nouvel état en paramètre.
* `void setEnabled(bool state)` : Active ou désactive le composant. Un composant désactivé ne répond pas au toucher et son apparence est modifiée (généralement grisée) pour l'indiquer.

## 5. Gestion des Événements

La `UICheckbox` répond à l'événement `handleRelease`. Lorsqu'un utilisateur touche et relâche le doigt à l'intérieur de la zone du composant :

1. L'état interne `_checked` est inversé.
2. Si un callback a été défini avec `onToggle()`, il est appelé avec le nouvel état.
3. Le composant se redessine pour refléter son nouvel état.

## 6. Exemple d'Utilisation Complet

Voici un exemple tiré de la classe `SettingsScreen` qui montre comment créer et utiliser une `UICheckbox`.

```cpp
// Dans la méthode init() de votre classe qui hérite de UIScreen

// 1. Définir un style pour la checkbox
UICheckboxStyle checkboxStyle;
checkboxStyle.labelStyle.textColor = currentTheme.textColor; // Utilise les couleurs du thème
checkboxStyle.labelStyle.bgColor = currentTheme.backgroundColor;
checkboxStyle.boxColor = currentTheme.textColor;
checkboxStyle.checkColor = currentTheme.textColor;
checkboxStyle.boxSize = 24;
checkboxStyle.spacing = 10;

// 2. Instancier la checkbox
auto testCheckbox = new UICheckbox(
    tft,
    40, 190,               // x, y
    240, 40,               // w, h
    "Notification",        // texte
    checkboxStyle,         // style
    true                   // état initial (cochée)
);

// 3. Définir une action lorsque l'état change
testCheckbox->onToggle([](bool state) {
    Serial.print("UICheckbox state changed to: ");
    Serial.println(state ? "Checked" : "Unchecked");
});

// 4. Ajouter la checkbox à l'écran pour qu'elle soit gérée automatiquement
addComponent(std::unique_ptr<UIComponent>(testCheckbox));
```

Dans cet exemple, lorsque l'utilisateur clique sur la case "Notification", son état est inversé, un message est affiché sur le port série, et la case se redessine avec ou sans la coche.
