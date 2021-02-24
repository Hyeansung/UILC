# Uniform irradiation LED arrangement optimization using approximation of integral equation

Kim, Hyeaonsung

*School of physics and photonics, Gwangji Insititute of Scoence and Technology, 123 Chamdan St. Gwangju, Korea*

\* *qwqwhsnote@gm.gist.ac.kr*

**Abstract**

© 2021 Optical Society of America under the terms of the [OSA Open Access Publishing Agreement](https://www.osapublishing.org/library/license_v1.cfm)

## 1. Introduction

## 2. Mathematical Construction

Common LED intensity model is a Lambertian intensity distribution model[^Lamber] with an inverse square law.

$$I(r, \theta) = \frac{I_0}{r^2}\cos^m(\theta)$$

Where $I_0$ is an intensity that LED emits perpendicular to its surface, $\theta$ is an angle between perpendicular vector and direction vector to point from center of the LED, and $m$ is a number, determing the optical property. 

![geometrc](./Fig/Fig0.PNG)
**Fig. 1**  Geometrical Schematic diagram of target plane and LED array plane.$(x_i,y_i,0)$ is a i-th LED location coordinate on below plane. $(x_j,y_j,0)$ is a sample point on target plane above LED plane.

### 2-1. 1-dimension array

Consider 1-dimensional array of LED and line with a distance $H$ from the array surface. For, i-th LED and j-th sample point, the distance $r$ and view-angle $\theta$ can be calculated as next with a location coordinate value $x,t$ of each line whose origins are located on same perpendicular line.

$$r_{ij} = \sqrt{H^2 + (x_i -t)^2}$$

$$\theta_{i} = \arctan(\frac{|x_i - t|}{H})$$

With a Lambertian distribution model, the intensity of the j-th sample point $I$ is

$$I(t) := \sum_{i=1}^N I(x_i,t)= I_0 \sum_{i=1}^N \frac{H^m}{(H^2 + (x_i - t)^2)^(\frac{m}{2} +1)}$$

for $N$ LEDs. To reduce the complexity of the calculation we consider all LEDs have same optical properties in radiation power and distribution.

Now, assume that the LEDs are located on closed region which length is $W$ and we only consider the intensity in that region on above line. In other word, it means $x, t \in [-\frac{W}{2}, \frac{W}{2}]$. Using Kronecker delta function, we can represent the specific arrangement of the LEDs as array function $\sigma$ and define the function $f(x,t)$ as next.

$$\sigma(x) := \sum_{i=1}^N \delta(x-x_i)$$
$$f(x,t) := \frac{H^m}{(H^2 + (x - t)^2)^(\frac{m}{2} +1)}$$

Our goal is finding a specific arrangement $\{x_i \}_{i=1}^N$. However, the number of the LED $N$ is not important. We will concentrate on tendency of the $\sigma(x)$ by $x$ values. Intuitively, we can infer some properties of the function $\sigma(x)$. For example, it will show narrow distance between two consecutive $x_n$, $x_{n+1}$ near boundary than they are located near center, because, with a constant distance case; **Morena**, it showed smaller intensity value near the boundary. 


## 3. Experiment 

## 4. Result and discussion


## 5. Conclusion


## References

[^Lamber]: D. Wood, Optoelectronic Semiconductor Devices, Prentice-Hall international series in optoelectronics (Prentice Hall,
1994).



