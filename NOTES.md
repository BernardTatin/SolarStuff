# NOTES

## remplacement de `select` par `poll`

Il y a deux raisons:

- la page de manuel de `select` indique que seuls les descripteurs de valeurs inférieure à `FD_SETSIZE` soit 1024 sont pris en compte et que les applications modernes doivent se tourner vers `poll`, et mon application est mderne,
- `poll` permet un code plus clair, plus simple.

Il n'y a pas de différence de comportement entre les deux fonctions, sauf peut-être en cas d'erreurs liées à des ressources trop faibles.

### et à quoi ça sert?

A l'époque où j'ai commencé ce code, c'était la seule manière simple et élégante de demander un rafraîchissement de la fenêtre toutes les N millisecondes.

Le principe est simple:

- on récupère le descripteur de fichier de la session **X** (`int select_fd = XConnectionNumber(xconf_main.display);`),
- la fonction `do_select` utilise `poll` qui retourne soit:
    - en cas de dépassement de temps,
    - en cas d'activité sur le canal.


