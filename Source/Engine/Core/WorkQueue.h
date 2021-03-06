//
// Copyright (c) 2008-2014 the Urho3D project.
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

#include "List.h"
#include "Mutex.h"
#include "Object.h"

namespace Urho3D
{

/// Work item completed event.
EVENT(E_WORKITEMCOMPLETED, WorkItemCompleted)
{
    PARAM(P_ITEM, Item);                        // WorkItem ptr
}

class WorkerThread;

/// Work queue item.
struct WorkItem
{
    // Construct
    WorkItem() :
        priority_(M_MAX_UNSIGNED),
        sendEvent_(false),
        completed_(false)
    {
    }
    
    /// Work function. Called with the work item and thread index (0 = main thread) as parameters.
    void (*workFunction_)(const WorkItem*, unsigned);
    /// Data start pointer.
    void* start_;
    /// Data end pointer.
    void* end_;
    /// Auxiliary data pointer.
    void* aux_;
    /// Priority. Higher value = will be completed first.
    unsigned priority_;
    /// Whether to send event on completion.
    bool sendEvent_;
    /// Completed flag.
    volatile bool completed_;
};

/// Work queue subsystem for multithreading.
class URHO3D_API WorkQueue : public Object
{
    OBJECT(WorkQueue);
    
    friend class WorkerThread;
    
public:
    /// Construct.
    WorkQueue(Context* context);
    /// Destruct.
    ~WorkQueue();
    
    /// Create worker threads. Can only be called once.
    void CreateThreads(unsigned numThreads);
    /// Add a work item and resume worker threads.
    void AddWorkItem(const WorkItem& item);
    /// Pause worker threads.
    void Pause();
    /// Resume worker threads.
    void Resume();
    /// Finish all queued work which has at least the specified priority. Main thread will also execute priority work. Pause worker threads if no more work remains.
    void Complete(unsigned priority);
    
    /// Return number of worker threads.
    unsigned GetNumThreads() const { return threads_.Size(); }
    /// Return whether all work with at least the specified priority is finished.
    bool IsCompleted(unsigned priority) const;
    
private:
    /// Process work items until shut down. Called by the worker threads.
    void ProcessItems(unsigned threadIndex);
    /// Purge completed work items and send completion events as necessary.
    void PurgeCompleted();
    /// Handle frame start event. Purge completed work from the main thread queue, and perform work if no threads at all.
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
    
    /// Worker threads.
    Vector<SharedPtr<WorkerThread> > threads_;
    /// Work item collection. Accessed only by the main thread.
    List<WorkItem> workItems_;
    /// Work item prioritized queue for worker threads. Pointers are guaranteed to be valid (point to workItems.)
    List<WorkItem*> queue_;
    /// Worker queue mutex.
    Mutex queueMutex_;
    /// Shutting down flag.
    volatile bool shutDown_;
    /// Pausing flag. Indicates the worker threads should not contend for the queue mutex.
    volatile bool pausing_;
    /// Paused flag. Indicates the queue mutex being locked to prevent worker threads using up CPU time.
    bool paused_;
};

}
