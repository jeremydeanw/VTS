import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

def mult (k, tuple):
	return (k*tuple[0], k*tuple[1])
	
def plus (*tuples):
	x = 0.
	y = 0.
	for a in tuples:
		x += a[0]
		y += a[1]
	return (x,y)

b0 = (0.,0.)
b1 = (2.5,1.)
b2 = (4.,1.)
b3 = (5.5,0.)

d0 = (11.5,0.)
d1 = (9.0,-1.)
d2 = (7.5,-1.)
d3 = (6.,0.)

b_0 = b0
b_1 = plus( mult(1./12.,b3) ,mult(-3./12.,b2), b1, mult(5./12., d3), mult(-3./12., d2) )
b_2 = plus( mult(2./12.,b3) ,mult(6./12.,b2), mult(10./12., d3), mult(-6./12., d2) )
b_3 = mult(0.5, plus(b3,d3)) 

d_0 = d0
d_1 = plus( mult(1./12.,d3) ,mult(-3./12.,d2), d1, mult(5./12., b3), mult(-3./12., b2) )
d_2 = plus( mult(2./12.,d3) ,mult(6./12.,d2), mult(10./12., b3), mult(-6./12., b2) )
d_3 = mult(0.5, plus(b3,d3)) 

# print tuple(b_0)
# print tuple(b_1)
# print tuple(b_2)
# print tuple(b_3)

verts1 = [
    b0,b1,b2,b3
    ]

verts2 = [
	d0,d1,d2,d3
	]
	
verts_1 = [
	b_0,b_1,b_2,b_3
	]

verts_2 = [
	d_0,d_1,d_2,d_3
	]

codes = [Path.MOVETO,
         Path.CURVE4,
         Path.CURVE4,
         Path.CURVE4,
         ]

path1 = Path(verts1, codes)
path2 = Path(verts2, codes)

path_1 = Path(verts_1, codes)
path_2 = Path(verts_2, codes)

fig = plt.figure()
ax = fig.add_subplot(111)

patch1 = patches.PathPatch(path1, facecolor='none', lw=2)
patch2 = patches.PathPatch(path2, facecolor='none', lw=2)
ax.add_patch(patch1)
ax.add_patch(patch2)

patch_1 = patches.PathPatch(path_1, facecolor='none', edgecolor='red', lw=2)
patch_2 = patches.PathPatch(path_2, facecolor='none', edgecolor='red', lw=2)
ax.add_patch(patch_1)
ax.add_patch(patch_2)

xs1, ys1 = zip(*verts1)
ax.plot(xs1, ys1, 'x--', lw=2, color='black', ms=10)

xs2, ys2 = zip(*verts2)
ax.plot(xs2, ys2, 'x--', lw=2, color='black', ms=10)

xs_1, ys_1 = zip(*verts_1)
ax.plot(xs_1, ys_1, 'x--', lw=2, color='red', ms=10)

xs_2, ys_2 = zip(*verts_2)
ax.plot(xs_2, ys_2, 'x--', lw=2, color='red', ms=10)

ax.text(b0[0], b0[1], 'b0')
ax.text(b1[0], b1[1], 'b1')
ax.text(b2[0], b2[1], 'b2')
ax.text(b3[0], b3[1], 'b3')

ax.text(d0[0], d0[1], 'd0')
ax.text(d1[0], d1[1], 'd1')
ax.text(d2[0], d2[1], 'd2')
ax.text(d3[0], d3[1], 'd3')

ax.text(b_0[0], b_0[1], 'b0~', color='red')
ax.text(b_1[0], b_1[1], 'b1~', color='red')
ax.text(b_2[0], b_2[1], 'b2~', color='red')
ax.text(b_3[0], b_3[1], 'b3~', color='red')

ax.text(d_0[0], d_0[1], 'd0~', color='red')
ax.text(d_1[0], d_1[1], 'd1~', color='red')
ax.text(d_2[0], d_2[1], 'd2~', color='red')
ax.text(d_3[0], d_3[1], 'd3~', color='red')

ax.set_xlim(-1., 14.)
ax.set_ylim(-5., 5.)
ax.set_title('Bezier Curve Joins')
plt.show()