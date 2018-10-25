# Formato para grafos desde archivos

Se aceptan los siguientes formatos:

_Los números entre corchetes son el número de línea_

### Formato 1 (sin nombre de nodos)
```
[1]  número de nodos
[2]  no dirigido (0), dirigido (1)
[3]  arista: nodo inicial, nodo final, peso
[4]  arista...
[...]
```
#### Ejemplo*
```
[1]  4
[2]  0
[3]  A C 3
[4]  C B 2
[5]  D B 9
[6]  B A 6
[7]  A D 5
```


### Formato 2 (con nombre de nodos)
```
[1]   número de nodos
[2]   no dirigido (0), dirigido (1)
[3]   nodo 1
[4]   nodo 2
[...]
[n+2] nodo n
[n+3] arista: nodo inicial, nodo final, peso
[n+4] arista...
[...]
```
#### Ejemplo*
```
[1]  4
[2]  0
[3]  X
[4]  Y
[5]  Z
[6]  W
[7]  X Z 3
[8]  Z Y 2
[9]  W Y 9
[10] Y X 6
[11] X W 5
```

\*Aquí, el tipo de dato de los nodos es `char`

Los ejemplos se encuentran en los archivos de la carpeta `txt_graphs`

**Nota:** El tipo de dato del nombre de los nodos dependerá del valor de N en [esta](https://github.com/UTEC-PE/project-grafin/blob/a56d42ce20af02f29c928e7dee25768262172061/graph.h#L20) línea 