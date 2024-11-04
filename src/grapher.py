import numpy as np
import matplotlib.pyplot as plt

def leerPuntosOriginales():
    puntosOriginales = np.loadtxt("/home/angel/Documentos/UCSP/Semestre_Nivelacion/EstructuraDatosAvanzados/11.PCA/puntosXYZ_Originales.txt")
    x = puntosOriginales[:,0]
    y = puntosOriginales[:,1]
    z = puntosOriginales[:,2]
    return x,y,z

def leerPuntosXYZ():
    puntosXYZ = np.loadtxt("/home/angel/Documentos/UCSP/Semestre_Nivelacion/EstructuraDatosAvanzados/11.PCA/puntosXYZ.txt")
    x = puntosXYZ[:,0]
    y = puntosXYZ[:,1]
    z = puntosXYZ[:,2]
    return x,y,z

def leerRectaPCA():
    rectaPCA = np.loadtxt("/home/angel/Documentos/UCSP/Semestre_Nivelacion/EstructuraDatosAvanzados/11.PCA/rectaPCA.txt")
    return rectaPCA

def leerProyecciones():
    proyecciones = np.loadtxt("/home/angel/Documentos/UCSP/Semestre_Nivelacion/EstructuraDatosAvanzados/11.PCA/proyeccion.txt")
    x = proyecciones[:,0]
    y = proyecciones[:,1]
    z = proyecciones[:,2]
    return x,y,z

def leerProyecciones2():
    proyecciones = np.loadtxt("/home/angel/Documentos/UCSP/Semestre_Nivelacion/EstructuraDatosAvanzados/11.PCA/reProyeccion.txt")
    x = proyecciones[:,0]
    y = proyecciones[:,1]
    z = proyecciones[:,2]
    return x,y,z

##################################
###########   MAIN   #############
##################################

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')


# Graficar puntos centrados
x,y,z = leerPuntosXYZ()
ax.scatter(x,y,z)

# Graficar recta PCA
recta = leerRectaPCA()
# --> Para el grafico oficial
'''
ax.plot3D(  [-500*recta[0], 500*recta[0]],
            [-500*recta[1], 500*recta[1]], 
            [-500*recta[2], 500*recta[2]], 'black' )      
'''

# --> Para el grafico de la presentacion

ax.plot3D(  [-7*recta[0], 7*recta[0]],
            [-7*recta[1], 7*recta[1]], 
            [-7*recta[2], 7*recta[2]], 'black' )

            
# Graficar proyecciones
xP,yP,zP = leerProyecciones()
#ax.scatter(xP,yP,zP)
for i in range (len(xP)):
    xAux = x[i]
    yAux = y[i]
    zAux = z[i]
    ax.plot3D( [xAux, xP[i]],
               [yAux, yP[i]],
               [zAux, zP[i]], 'red')

'''            
# Graficar puntos originales
x_o,y_o,z_o = leerPuntosOriginales()
ax.scatter(x_o,y_o,z_o)

# Graficar proyecciones de los puntos originales
x_p,y_p,z_p = leerProyecciones2()
ax.scatter(x_p,y_p,z_p)
'''

plt.show()