# Sistema Editor-Suscriptor

**Asignatura:** Sistemas Distribuidos

**Curso:** 2015/2016

**Autores:** [Daniel Melero Chaves] (https://github.com/DanielMChaves)

**Lenguaje:** C

**Proposito:** El objetivo principal es que el alumno pueda ver de una forma aplicada qué infraestructura básica requiere una arquitectura de comunicación basada en un modelo editor-subscriptor.

Para ello, se plantea desarrollar un esquema de este tipo con las siguientes características específicas (Nota: puede ser conveniente que repase el material teórico de la asignatura para refrescar la terminología y los conceptos asociados a este esquema de interacción):

- Se va a seguir un modelo basado en temas.
- Se utilizará un esquema con un único proceso que actúa como intermediario proporcionando el desacoplamiento espacial entre los editores y los subscriptores. El intermediario se encarga de propagar cada evento generado por un editor a los subscriptores interesados en ese tipo de evento (tema).
- Va a usarse un esquema de tipo push. Como se irá viendo a lo largo del enunciado, y como se ha explicado en la parte teórica de la asignatura, la implementación de este tipo de comunicación a contracorriente (de tipo callback) y asíncrona es uno de los aspectos más complejos de construir en este esquema (la sección Manejo de eventos en el subscriptor estudia en detalle el problema).
- En cuanto al contenido de un evento, se propone un esquema muy sencillo. Un evento se caracterizará por un nombre de tema y un único valor asociado, correspondiéndose ambos elementos con strings que contienen cualquier carácter exceptuando un espacio.

En una primera versión básica de la práctica, no va a existir la posibilidad de creación dinámica de temas (tipos de eventos). En el arranque del sistema, el intermediario será informado mediante un fichero de qué temas existen en el sistema y éstos serán los únicos disponibles para editores y subscriptores.
En la versión avanzada, se permitirá la creación dinámica de temas, tal como se describirá en la sección dedicada a esa versión.

En cuanto a las tecnologías de comunicación usadas en la práctica, se utilizarán sockets de tipo stream y se supondrá un entorno de máquinas heterogéneas.
