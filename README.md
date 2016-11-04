# GPU-accelerated Modeling of Laplacian Diffusion

### Overview
Diffusion is the movement of a substance across a concentration gradient; it continues until a (temperature / pressure / density) equilibrium has been reached. The diffusion equation is a partial differential equation (PDE) that describes density dynamics. If D, the diffusion coefficient (a term describing the rate of diffusion) varies depending on the density, then the equation is nonlinear. However, if the rate of diffusion is a constant, then the diffusion equation reduces to a linear equation, the well-known heat distribution equation.

### Task
One characteristic of the finite difference solution to the diffusion problem, that can be exploited when implementing a parallel solution, is the independence of the calculations. They may be performed in any order and still be correct (i.e. there are only local data dependencies). Of course, you still have to isolate old/new temperature/density values.

### Requirements / Deliverables
Perform simple diffusion modeling of any of the scenarios presented above.
* You must use CUDA executing on a GPU; Seawolf and Quattro are available.
* Graph the change in density over time at a particular point in the cell. Create a visualization of the diffusion over time, essentially a heat map. There are tools available in other languages (R, Python, OpenGL). You can also directly create an image file (pgm, svg).
* Submit a hard-copy of your source-code, graph/visualization, and analysis. Be prepared to present your results in class.
