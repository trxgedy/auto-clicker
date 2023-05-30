#ifndef bind_hpp
#define bind_hpp

namespace bind
{
	class c_bind
	{
	public:
		inline static int left_bind  { 0x0 };
		inline static int right_bind { 0x0 };
		inline static bool same_bind { false };

		std::string left_bind_msg  { "None" };
		std::string right_bind_msg { "None" };

	public:
		c_bind( ) = default;
		~c_bind( ) = default;

		auto keybinder( int &side, std::string &msg ) -> void;
	};
}

#endif // !bind_hpp