# **Task Scheduler**

Creates a list of tasks and executes them at their specified time, tasks can be one off or recurring.

- Tasks can be created and assigned to any remote switch.
- Each task can contain a single command and connect to a single switch
- Tasks are executed in the order in which they were added
- When a task which is not recurring is complete, it is removed (not implemented)

- In the more recent updates, the main schedule loop runs on its own thread while other utilities will have their own threads.
