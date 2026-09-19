# From Flying Things to Embedded Systems
## Chapter I: The Child

Since I was a child, I was always fascinated with flying things.

I remember my parents buying me LEGO sets so I could tear them apart and make a cool plane out of them. I ended up having so many pieces in a cardboard box that the box itself weighed more than I could lift at the time.

I could spend entire afternoons just rummaging through that box, so much so that the tips of my fingers would develop calluses from the repeated friction between the hard plastic surfaces and my child-like fingers. My mum would get very annoyed every single day, but I think deep inside, she enjoyed seeing me having fun.

Around that time, my parents also took my siblings and me to fly an RC plane. I had so much fun with that thing. Later that day, we went near an airport to watch commercial planes take off and land for hours.

From that time on, my love for airplanes just kept getting bigger, as if a little seed of love had been implanted in my brain.

I don't really know what initially made me love aviation so much. Maybe it was the engineering behind it. There was something fascinating about seeing something so complex gracefully flying into the horizon. Military aviation was especially cool to me: aircraft capable of reaching supersonic speeds, sustaining incredibly complex forces, beautiful designs, the connection between pilot and aircraft, and the way their weapon systems worked.

Whatever the reason, the seed was there.

## Chapter II:  Other Seeds

However, that little seed was overshadowed for a long time.

I kept growing up, and my curiosity instinctively kept growing other seeds. Some of them naturally died, others kept going for longer until I eventually cut them off, and some keep growing to this day.

Music became one of them. I got into composition, learned instruments, and eventually even finished a music degree. Martial arts, drawing, and other hobbies also became part of my life.

All of those things shifted the focus away from my love for aviation for quite some time, until I reached around sixteen or seventeen.

## Chapter III:  The F-16

Then, I discovered the world of aviation simulators... and DCS. I became obsessed with DCS, so much so that I could spend entire days just watching people dogfighting in their sims on YouTube.

One F-16 against another F-16. Three F-15s against five MiG-29s. One F-22 against ten Su-27s.

Entire playlists eaten away by my curiosity. The F-16 in particular became my favorite. Small, powerful, and beautiful. There was something about that aircraft that just clicked with me.

I had also wanted to become a pilot. Unfortunately, myopia meant that wasn't really an option for me. So, when I was eighteen or nineteen, I decided to at least try to experience some of that world for myself. With my own money, I bought a HOTAS so I could try flying an F-16 on my PC.

There was only one problem: My PC was devastatingly slow. Eventually, the HOTAS ended up in a wardrobe. 

The dream of flying didn't disappear, though. It simply remained somewhere in the background.

## Chapter IV:  The Question

While finishing my computer science degree, I realized that building full-stack applications, designing complete architectural systems, and making all of that work reliably was really cool.

I was good at it, but eventually, I realized that I hadn't asked the right question all this time: How do aircraft systems work?

My curiosity naturally sent me down another rabbit hole. I started watching documentaries about aircraft accidents, how systems failed, how they were supposed to work, how engineers reworked them, and why those changes were necessary.

I became interested in the aircraft as a whole. ot just how to fly one, but how all of its systems interacted with each other and how such an incredibly complex machine could actually function.

I wasn't yet digging deeply into the electronics or software behind those systems. My curiosity was still mostly at the systems level, but eventually, that curiosity led me to the world of embedded systems. At the time, it felt like something far too big for me to grasp.

## Chapter V:  The Detour

By the time I finished university and started looking for jobs, I was a bit tired from all the learning. I just wanted to get a job and start earning money.

So that's what I did.

I started looking through all sorts of companies and jobs. Since it was around the late-COVID era, jobs were really scarce for software engineers.

Eventually, I found a job that was somewhat related to embedded software engineering. I ended up working on the web frontend and backend of an embedded device for home automation. Web development was fine, and I was really good at it, don't get me wrong.

But being around other teams working directly on the embedded projects made something increasingly clear to me. That was the path I wanted to follow.

## Chapter VI:  The Click

When I first joined the company, we had a few months of learning before moving onto our actual teams.

During that period, I got to write C drivers for some of the devices the company was developing. Nothing particularly complicated. Some simple blinkers, really. But I was using their libraries and, more importantly, I was working with the physical devices themselves.

I wrote code, the device did something, I could physically see it happen, and something just clicked in my brain.

After spending so much time working with software, where everything ultimately lived behind layers of abstractions, there was something incredibly satisfying about writing code and making an actual physical device do something in the real world.

That short experience made my love for embedded development grow significantly. Unfortunately, it didn't last very long. I eventually moved onto the team I was going to work with, which specializes in the screens of the embedded devices.

The screens themselves are incredibly complex, based on Linux, and the driver team is just insanely good at what they do. My day-to-day work is mostly focused on user interaction and functionality, so I don't actually interact with much embedded development directly. Still, being exposed to that world was enough.

I had found another seed. And this one was growing.

## Chapter VII:  The Idea

While working there, I started thinking about all of this: I had spent years loving aviation, I had spent years learning software engineering, I had become fascinated with how complex aircraft systems worked and I had recently discovered how much I enjoyed making software interact with physical hardware. Eventually, I decided to gather all of those things I loved and start a project.

The goal isn't to suddenly become a senior embedded developer overnight, it is much simpler than that. I want to build something complex enough that I can genuinely say: "Hey, I kinda know what's embedded about." Something that lets me learn step by step, without becoming so large that I never actually finish it.

## Chapter VIII:  The Black Box

So, what do I actually want to build? An RC aircraft black box.

The idea is relatively simple: build a small device that can be placed inside an RC plane and record what happens during a flight. I want it to collect data from a variety of sensors, process and combine that data, store everything locally, and then allow me to inspect the flight afterwards from a computer or a phone.

At a high level, the system would look something like this:

Sensors → Data Fusion → SD Card Storage → Wi-Fi / Bluetooth / SD Transfer → Web Interface

I don't want to completely define the final hardware before I start. I want the project to evolve as I learn more about the components, their limitations, and what information is actually useful when trying to understand what an aircraft is doing during a flight.

The black box should eventually be able to record things such as position, altitude, acceleration, orientation, pressure, temperature, and potentially electrical data from the aircraft.

All of that data would be stored during the flight, preferably without depending on a network connection. After landing, I want to be able to retrieve the data and inspect the flight through a web interface.

Ideally, I should be able to see the path the aircraft took, examine the sensor readings throughout the flight, and correlate different measurements with one another.

I want to finally start answering the question that has been sitting in the back of my head for years: How does this thing actually work?
