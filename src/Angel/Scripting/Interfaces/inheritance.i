%include <factory.i>

%factory(DeveloperLog*, ConsoleLog, CompoundLog, FileLog, SystemLog);
%factory(Actor*, TextActor, Camera, PhysicsActor, ParticleActor);
%factory(Renderable*, TextActor, PhysicsActor, Camera, ParticleActor, Actor);
%factory(MessageListener*, TextActor, PhysicsActor, Camera, ParticleActor, Actor);