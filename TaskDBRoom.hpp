#ifndef TaskDBRoom_hpp__
#define TaskDBRoom_hpp__

class TaskDBRoom : public Tasks::Task
{
public:
	TaskDBRoom( )
	{
	}

	virtual std::string GetDesc( ) const {
		static char pcBuffer[ 512 ];
		sprintf( pcBuffer, "TaskDBRoom : ." );
		return pcBuffer;
	}

	virtual void Proc( )
	{
	}

protected:

};

#endif // TaskNpcWarper_hpp__