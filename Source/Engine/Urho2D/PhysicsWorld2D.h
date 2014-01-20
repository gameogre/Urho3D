//
// Copyright (c) 2008-2013 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "Component.h"

#include <Box2D/Box2D.h>

namespace Urho3D
{

/// Debug draw flag.
enum DebugDrawFlag
{
    DEBUGDRAW_SHAPE = b2Draw::e_shapeBit,
    DEBUGDRAW_JOINT = b2Draw::e_jointBit,
    DEBUGDRAW_AABB = b2Draw::e_aabbBit,
    DEBUGDRAW_PAIR = b2Draw::e_pairBit,
    DEBUGDRAW_CENTEROFMASS = b2Draw::e_centerOfMassBit,
};

/// 2D physics simulation world component. Should be added only to the root scene node.
class URHO3D_API PhysicsWorld2D : public Component, public b2ContactListener, public b2Draw
{
    OBJECT(PhysicsWorld2D);

public:
    /// Construct.
    PhysicsWorld2D(Context* scontext);
    /// Destruct.
    virtual ~PhysicsWorld2D();
    /// Register object factory.
    static void RegisterObject(Context* context);

    /// Visualize the component as debug geometry.
    virtual void DrawDebugGeometry(DebugRenderer* debug, bool depthTest);

    // From b2ContactListener.
    /// Called when two fixtures begin to touch.
    virtual void BeginContact(b2Contact* contact);
    /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact);
    /// This is called after a contact is updated.
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    /// This lets you inspect a contact after the solver is finished.
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    // From b2Draw.
    /// Draw a closed polygon provided in CCW order.
    virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    /// Draw a solid closed polygon provided in CCW order.
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    /// Draw a circle.
    virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    /// Draw a solid circle.
    virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
    /// Draw a line segment.
    virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    /// Draw a transform. Choose your own length scale.
    virtual void DrawTransform(const b2Transform& xf);

    /// Set allow sleeping.
    void SetAllowSleeping(bool enable);
    /// Set warm starting.
    void SetWarmStarting(bool enable);
    /// Set continuous physics.
    void SetContinuousPhysics(bool enable);
    /// Set sub stepping.
    void SetSubStepping(bool enable);
    /// Set gravity.
    void SetGravity(const Vector2& gravity);
    /// Set auto clear forces.
    void SetAutoClearForces(bool enable);
    /// Set debug draw flags.    
    void SetDebugDrawFlags(unsigned debugDrawFlags);
    /// Set velocity iterations.
    void SetVelocityIterations(unsigned velocityIterations);
    /// Set position iterations.
    void SetPositionIterations(unsigned positionIterations);

    /// Step the simulation forward.
    void Update(float timeStep);
    /// Add debug geometry to the debug renderer.
    void DrawDebugGeometry(bool depthTest);
    /// Set debug renderer to use. Called both by PhysicsWorld2D itself and physics components.
    void SetDebugRenderer(DebugRenderer* debug);
    /// Set debug geometry depth test mode. Called both by PhysicsWorld2D itself and physics components.
    void SetDebugDepthTest(bool enable);

    /// Return allow sleeping.
    bool GetAllowSleeping() const;
    /// Return warm starting.
    bool GetWarmStarting() const;
    /// Return continuous physics.
    bool GetContinuousPhysics() const;
    /// Return sub stepping.
    bool GetSubStepping() const;
    /// Return auto clear forces.
    bool GetAutoClearForces() const;
    /// Return gravity.
    const Vector2& GetGravity() const { return gravity_; }
    /// Return debug draw flags.
    unsigned GetDebugDrawFlags() const { return m_drawFlags; }
    /// Return velocity iterations.
    unsigned GetVelocityIterations() const { return velocityIterations_; }
    /// Return position iterations.
    unsigned GetPositionIterations() const { return positionIterations_; }

    /// Return the Box2D physics world.
    b2World* GetWorld() { return world_; }

protected:
    /// Handle node being assigned.
    virtual void OnNodeSet(Node* node);

private:
    /// Handle the scene subsystem update event, step simulation here.
    void HandleSceneSubsystemUpdate(StringHash eventType, VariantMap& eventData);

    /// Box2D physics world.
    b2World* world_;
    /// Gravity.
    Vector2 gravity_;
    /// Velocity iterations.
    unsigned velocityIterations_;
    /// Position iterations.
    unsigned positionIterations_;

    /// Extra weak pointer to scene to allow for cleanup in case the world is destroyed before other components.
    WeakPtr<Scene> scene_;
    /// Debug renderer.
    DebugRenderer* debugRenderer_;
    /// Debug draw depth test mode.
    bool debugDepthTest_;
};

}
