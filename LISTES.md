# des listes à la Lisp en C

Je voulais me faire un système de liste comme celui de Lisp. Je suis parti de deux structures:

```C
// les éléments de la liste
typedef struct _TScl_element {
	void *value;
	struct _TScl_element *next;
} TScl_element;
// la liste
typedef struct _TScl_list {
	TScl_element *first;
} TScl_list;
```

Beeennn... Faut pas faire comme ça - mais je le savais déjà, ma solution à l'avantage d'être rapide et facile à mettre en oeuvre.

La structure fondamentale des Lisp, c'est le `cons` qui pointe vers une valeur, le `car` (ou `head`, `first`) et un suivant, le `cdr` (ou `rest`, `tail`). En fait, j'y suis presque avec mon `TScl_element` (aux noms près), je n'est pas besoin de `TScl_list`. Ma construction est centrèe sur la partie *liste* alors qu'elle devrait être centrée sur la partie *élément*.

En lisant la littérature autour de la conception des listes en Lisp, je comprends bien que mon element a besoin de donnée supplémentaires pour que l'utilisation de mes listes soient plus... ou plutôt moins rock'n'roll.

Pour l'instant, les seules fonctions sur mes listes que j'ai implémenté sont:

```C
static inline TScl_list *cl_list_new(void) {
	TScl_list *list = (TScl_list *)calloc(1, sizeof(TScl_list));
	return list;
}

static inline void cl_list_add(TScl_list *list, TScl_element *elt) {
    elt->next = list->first;
    list->first = elt;
}

TScl_list *cl_reverse(TScl_list *list);
void cl_list_for_each(TScl_list *list, void (*on_element)(TScl_element *elt));
void cl_list_free(TScl_list *list, void (*free_value)(void *value));
```

La fonction `cl_reverse` renvoit une nouvelle liste dont l'ordre est inversé par rapport à la liste d'entrée. Au final, nous avons deux listes. Les problèmes arrivent au moment de la libération de la mémoire.