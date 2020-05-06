# Dijkstra
Proyecto escrito en c++ que aplica el algoritmo de dijkstra sobre un grafo definido estáticamentea a partir del siguiente enunciado:

 *En la red de la figura 1 mostrada a continuación, se ha indicado o colocado en cada enlace la probabilidad de que el enlace esté operativo (de que no fallen).*
 
 ![grafo](https://raw.githubusercontent.com/dulcedmgd/dijkstra/master/grafo.png)
 
 *Teniendo en cuenta que los fallos en enlaces diferentes son independientes entre sí, se pide: encuentre, elaborando un programa en C++, Encontrar, usando el algoritmo de Dijkstra, la ruta de máxima fiabilidad entre los nodos T y R, utilizando el algoritmo de Dijkstra.*
 
 **Nota: Debido a que dijkstra halla caminos mínimos y en el enunciado piden hallar la ruta de máxima fiabilidad, se han modificado los pesos de las aristas o enlaces para poder aplicar el algoritmo, esto porque dijkstra no halla rutas máximas, las modificaciones siguen el siguiente criterio:**
 
 * Se sabe que los pesos del grafo anterior indican la probabilidad de que el enlace esté operativo y que la probabilidad total es igual a 1, restaremos a la probabilidad total el peso de cada enlace, para hallar la probabilidad de que el enlace no esté operativo, de la siguiente manera: 1 - peso_de_la_arista,
 * Teniendo en los enlaces los nuevos pesos que indican la probabilidad de fallo del mismo, se aplicará dijkstra para hallar el camino mínimo de fallo, lo que se traduce al camino de máxima fiabilidad.
 
 
 ***Autor:** Dulce María García Duarte*
