from system import Body, System

AU = 1.5e11                 # Earth-Sun distance
Me = 5.972e24               # Earth mass
sec_per_day = 24 * 60 * 60  # Seconds per day
dt = 1 * sec_per_day
t_end = 365 * 11 * dt

# Solar system
system = System([
    Body(name='Sun',     mass=333000*Me, position=[0, 0],       velocity=[0, 0]),
    Body(name='Mercury', mass=0.0553*Me, position=[0.4*AU, 0],  velocity=[0, 47000]),
    Body(name='Venus',   mass=0.815*Me,  position=[0.72*AU, 0], velocity=[0, 35000]),
    Body(name='Earth',   mass=Me,        position=[AU, 0],      velocity=[0, 29290]),
    Body(name='Mars',    mass=0.107*Me,  position=[1.5*AU, 0],  velocity=[0, 24000]),
    Body(name='Jupiter', mass=317.8*Me,  position=[5.2*AU, 0],  velocity=[0, 12440]),
])

# Earth-Moon system
# system = System([
#     Body(name='Earth',   mass=Me,        position=[0, 0],       velocity=[0, 0]),
#     Body(name='Moon',    mass=0.0123*Me, position=[0.0025695*AU, 0], velocity=[0, 1000]),
# ])

t = 0
while t < t_end:
    system.update_state(dt)
    t += dt

system.plot()