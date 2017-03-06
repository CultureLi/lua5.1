
class T //tolua_export
{//tolua_export
private:
	int data;
public:
	T();
	static T& Instance();//tolua_export
	void Show();//tolua_export
	void SetData(int value);//tolua_export
	int GetData();//tolua_export
};//tolua_export