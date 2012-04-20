%include <factory.i>

%factory(DeveloperLog*, ConsoleLog, CompoundLog, FileLog, SystemLog);
%factory(Actor*, TextActor, MazeFinder, Camera, PhysicsActor, ParticleActor);
%factory(Renderable*, TextActor, MazeFinder, Camera, PhysicsActor, ParticleActor, Actor);
%factory(MessageListener*, TextActor, MazeFinder, Camera, PhysicsActor, ParticleActor, Actor);