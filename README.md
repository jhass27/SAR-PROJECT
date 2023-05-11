# DOCUMENTACIÓN SAR LA MERCED

Esta documentación del SAR es un compendio de todo lo trabajado para lograr el proyecto, te redigirá a los links necesarios para su desarrollo.

####   

#### Preparado por:

  

**Jhassmin Aricoché**

azucena.jhassmin@gmail[.com](mailto:clickup@email.com)

  

1\. Breve descripción
=====================

  

Esta documentación describe el proyecto SAR instalado en la Merced y es la segunda versión oficial del mismo. La primera versión fue desarrollada en el Radio Observatorio de Jicamarca y se instaló en Cuenca Huancavelica. Se pueden encontrar más detalles y documentación en los siguientes enlaces:

  

*   [https://drive.google.com/drive/folders/1f2i-h\_K9KSgQKXA7qWLCaLio\_-mBnykk?usp=sharing](https://drive.google.com/drive/folders/1f2i-h_K9KSgQKXA7qWLCaLio_-mBnykk?usp=sharing)

[https://docs.google.com/presentation/d/1QshgnE8KNi6dIY3vQXCJku8ymlbrmp94/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true](https://docs.google.com/presentation/d/1QshgnE8KNi6dIY3vQXCJku8ymlbrmp94/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true)

*   [https://drive.google.com/drive/folders/1f2i-h\_K9KSgQKXA7qWLCaLio\_-mBnykk?usp=sharing](https://drive.google.com/drive/folders/1f2i-h_K9KSgQKXA7qWLCaLio_-mBnykk?usp=sharing)

  

2\. Implementación del proyecto SAR LA MERCED
---------------------------------------------

  

Este segundo proyecto se basa en la primera versión del proyecto SAR, pero se han realizado algunas mejoras. Las principales diferencias están en los programas desarrollados y en el uso de la tarjeta UDOO como microcontrolador principal en lugar de una Raspberry Pi. La tarjeta UDOO también reemplaza al Arduino utilizado en la versión anterior, ya que incluye Arduino Leonardo en su sistema y puede operar esta parte de la tarjeta aunque el sistema operativo esté apagado.

[https://docs.google.com/document/d/13h16VUCDSRPiUex9ZiInCzO\_7my4w50Q/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true](https://docs.google.com/document/d/13h16VUCDSRPiUex9ZiInCzO_7my4w50Q/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true)

[https://drive.google.com/drive/folders/1oaSfVKCQK6dGG\_4N1ZZGlcl525pgHmAy?usp=sharing](https://drive.google.com/drive/folders/1oaSfVKCQK6dGG_4N1ZZGlcl525pgHmAy?usp=sharing)

  

El primer enlace describe cómo se implementó el radar SAR en la Merced, principalmente en cuanto al hardware. El segundo enlace lleva a una carpeta de desarrollo que detalla cómo se instalaron los programas y qué configuraciones se hicieron para que el radar funcione correctamente.

  

**Enlaces extras:**

  

[https://drive.google.com/drive/folders/19rDLyA2eiwhUxQ-mah2uFrJc-6Qegr3A?usp=sharing](https://drive.google.com/drive/folders/19rDLyA2eiwhUxQ-mah2uFrJc-6Qegr3A?usp=sharing)

[https://docs.google.com/document/d/1St8y6RLebBefKtwb-4OQORfvsViYNgh0/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true](https://docs.google.com/document/d/1St8y6RLebBefKtwb-4OQORfvsViYNgh0/edit?usp=sharing&ouid=113688688665892673850&rtpof=true&sd=true)

  

2.1. Desarrollo de los programas
--------------------------------

  

Los programas se desarrollaron en C y Python. Se creó un programa en C llamado `motor_pasos.ino` utilizando el entorno de desarrollo de Arduino. Este programa se subió a la UDOO como si fuera un script de Arduino Leonardo y se encuentra dentro de la carpeta "motor\_pasos" en la carpeta de scripts.

  

El programa principal es [`main.py`](http://main.py), que llama a las funciones del script [`vna.py`](http://vna.py) para controlar el Analizador de Red Vectorial (VNA). En este programa se definen los pulsos que se enviarán y los parámetros característicos de la medición.

  

3\. Conexión remota
-------------------

  

Se puede acceder al SAR de Cuenca y el Sar de la Merced en virtud de zerotier.

Para establecer conexión con Cuenca escribir:

  

ssh [pi@192.168.193.95](mailto:pi@192.168.193.95)

contraseña: jro-sar-PI

  

Para establecer conexión con La Merced escribir:

  

ssh [sar@192.168.193.206](mailto:sar@192.168.193.206)

contraseña: jro/sar/PI

  

Información adicional se encuentra en los siguientes links:

  

[https://drive.google.com/drive/folders/1KgowaVrweiVNX8\_Ow8teugrzGt56y7CN?usp=sharing](https://drive.google.com/drive/folders/1KgowaVrweiVNX8_Ow8teugrzGt56y7CN?usp=sharing)

  

[https://docs.google.com/document/d/1W6A9gsQkEKpcBOvqDGa\_X-H4pTHALA3r80Rw7-eK41Q/edit?usp=sharing](https://docs.google.com/document/d/1W6A9gsQkEKpcBOvqDGa_X-H4pTHALA3r80Rw7-eK41Q/edit?usp=sharing)

  

4\. **Procesamiento de imágenes**
---------------------------------

  

El programa de procesamiento de imágenes usa los datos h5 y los parámetros característicos de cada archivo para graficar las imágenes, basado en el algoritmo fdbp.

La diferencia entre el programa de La Merced y el de Cuenca radica en cómo se leen los archivos h5. Ya que los generados por el radar de la Merced son una versión actualizada y generada con pandas.

  

El programa de procesamiento en la Merced se encuentra aquí:

  

El programa de procesamiento en Cuenca se encuentra aquí:

  

[https://github.com/LuisDLCP/Landslide\_Project/tree/master2/src](https://github.com/LuisDLCP/Landslide_Project/tree/master2/src)

  

**5\. Desarrollo adicional para el SAR**
----------------------------------------

  

**Desarrollo de practicantes:** para el cambio de ángulo y para una aplicación de monitoreo usando telegram. Encontrará información en los siguientes enlaces:

  

[https://drive.google.com/drive/folders/1Ch8k7WyltC3u9nLRMoSnRVnC3z-7njv7?usp=sharing](https://drive.google.com/drive/folders/1Ch8k7WyltC3u9nLRMoSnRVnC3z-7njv7?usp=sharing)

[https://drive.google.com/drive/folders/1baAlhMkGqW9IrsJW6raXAgNhUCYkUavq?usp=sharing](https://drive.google.com/drive/folders/1baAlhMkGqW9IrsJW6raXAgNhUCYkUavq?usp=sharing)

  

**Desarrollo de monitoreo:** Con respecto al monitoreo se ha desarrollado un circuito integrado de una cámara espcam y telegram: Más información en el siguiente enlace:

  

[https://drive.google.com/drive/folders/1KgowaVrweiVNX8\_Ow8teugrzGt56y7CN?usp=sharing](https://drive.google.com/drive/folders/1KgowaVrweiVNX8_Ow8teugrzGt56y7CN?usp=sharing)

  

**5\. Mejoras del SAR La Merced con respecto al anterior SAR**
--------------------------------------------------------------

  

El principal objetivo del proyecto era entender y mejorar las capacidades del primer prototipo, tales como la determinación de la precisión, la variabilidad día/noche en las mediciones, la influencia de las estaciones y condiciones atmosféricas y la validación de las observaciones con mediciones complementarias, como mediciones con GPS, drones, etc.

  

Se han realizado mejoras respecto a la versión anterior, entre las que se pueden mencionar:

  

*   Se ha reducido el tamaño y peso de la platafoma móvil, de manera que el movimiento de las antenas es más estable y por ende se obtienen mediciones más confiables.

  

*   Se están utilizando unas antenas con un mayor ancho de haz, de esta manera las mediciones pueden abarcar mayores áreas del deslizamiento.

  

*   Se mejoró el sistema de movimiento de la plataforma, ahora es más robusto: ya que usa un sensor magnético en vez de uno mecánico y ya no se necesita de un microcontrolador para su funcionamiento. El nuevo mini computador incluye lo necesario para controlar el sistema.

  

*   La estructura metálica del radar SAR fue diseñada para en un futuro poder rotarla manualmente en azimuth, así que el radar posee ahora esa capacidad.

  

*   El reinicio del sistema se puede realizar remotamente y ya no necesita de un reinicio manual.