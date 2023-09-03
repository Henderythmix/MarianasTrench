#MarianaTrench
A submarine has a velocity of 8m/s when going downward initially. Due to buoyancy, we can determine that the sub will sink slower over time, and it has to sink down to 11034m.

We can determine a simplified formula of the endgame ship as followed:
$$
D(t)=11034(1-e^{-st})
$$
$$
v(t)=\frac{dD(t)}{dt}=11034se^{-st}
$$
$$
v(0)=8=11034se^{-(0)t}
$$
$$
11034s=8
$$
$$
s=\frac{8}{11034}
$$
$$
\therefore v(t)=8e^{-8t/11034}
$$

We however need to find a formula for the ship relative to its distance, because we are not going to keep track of time as we sink. We should find how the velocity and distance are related to each other. ~~Also numbers can get complicated if we use Euler's number.~~

$$
11034-11034e^{-st}=11034se^{-st}
$$
$$
D(t)=11034-\frac{v(t)}{s}
$$
$$
v(t)=-s(D(t)-11034)
$$
$$
\therefore v(d)=-s(d-11034)
$$
