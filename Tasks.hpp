#ifndef Tasks_hpp__
#define Tasks_hpp__

#include <stack>

namespace Tasks
{
	class Task
	{
	public:
		virtual ~Task( ) { }
		virtual std::string GetDesc( ) const = 0;

		virtual void Begin( bool bActive ) { }
		virtual void Pause( ) { }
		virtual void Resume( ) { }

		virtual void OnMoveComplete( bool bSuccess ) { }
		virtual void OnUnitSpawn( GameUnit* pUnit ) { }
		virtual void OnUnitMove( GameUnit* pUnit ) { }
		virtual void OnUnitClear( GameUnit* pUnit ) { }
		virtual void OnNpcNext( GameUnit* pUnit ) { }
		virtual void OnNpcClose( GameUnit* pUnit ) { }
		virtual void OnNpcMenu( GameUnit* pUnit, const std::vector<std::string>& rOptions ) { }
		virtual void OnMapChanged( ) { }

		virtual void Proc( ) = 0;

	};

	class EmptyTask : public Task
	{
	public:
		virtual std::string GetDesc( ) const { return "EmptyTask"; }
		virtual void Proc( ) { }

	};

	std::stack<Task*> g_rTasks;

	Task* Active( ) { return g_rTasks.top(); }

	void Push( Task* pTask )
	{
		printf( "---- Pushing a task! (%s)\n", pTask->GetDesc().c_str() );

		printf( "------- Pausing a task! (%s)\n", Active()->GetDesc().c_str() );
		Active( )->Pause( );

		g_rTasks.push( pTask );
		Active( )->Begin( true );

		printf( "------- Resuming a task! (%s)\n", Active()->GetDesc().c_str() );
		Active( )->Resume( );
	};

	void PushPaused( Task* pTask )
	{
		printf( "---- Pushing a paused task! (%s)\n", pTask->GetDesc().c_str() );

		printf( "------- Pausing a task! (%s)\n", Active()->GetDesc().c_str() );
		Active( )->Pause( );

		g_rTasks.push( pTask );
		Active( )->Begin( false );
	};

	void Pop( )
	{
		// We do not pop the last task (EmptyTask)
		if( g_rTasks.size() <= 1 ) return;
	
		printf( "---- Popping a task! (%s)\n", Active()->GetDesc().c_str() );

		printf( "------- Deleting a task! (%s)\n", Active()->GetDesc().c_str() );
		delete Active( );
		g_rTasks.pop();

		printf( "------- Resuming a task! (%s)\n", Active()->GetDesc().c_str() );
		Active( )->Resume( );
	}

	void PopAndPush( Task* pTask )
	{
		printf( "---- Pop-Pushing a paused task! (%s)\n", pTask->GetDesc().c_str() );

		// We do not pop the last task (EmptyTask)
		if( g_rTasks.size() > 1 ) {
	
			printf( "------- Deleting a task! (%s)\n", Active()->GetDesc().c_str() );

			delete Active( );
			g_rTasks.pop();
		}

		g_rTasks.push( pTask );
		Active( )->Begin( true );

		printf( "------- Resuming a task! (%s)\n", Active()->GetDesc().c_str() );
		Active( )->Resume( );

	}

	void Init( )
	{
		g_rTasks.push( new EmptyTask() );
	};


};

#endif // Tasks_hpp__