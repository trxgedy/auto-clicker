#ifndef menu_hpp
#define menu_hpp

extern auto ImGui_ImplWin32_WndProcHandler( HWND, UINT, WPARAM, LPARAM ) -> IMGUI_API LRESULT;

namespace menu
{
	struct context_t
	{
		HWND                    hwnd {};
		LPDIRECT3D9             d3d {};
		LPDIRECT3DDEVICE9       device {};
		D3DPRESENT_PARAMETERS	params {};

		WNDCLASSEXA wc {};
		bool context_state { true };
	};

	class c_window
	{
	private:
		static auto wnd_proc( HWND, UINT, WPARAM, LPARAM ) -> LRESULT WINAPI;
		auto cleanup_device( context_t &w_context ) -> void
		{
			if ( w_context.d3d )
			{
				w_context.d3d->Release( );
				w_context.d3d = nullptr;
			}

			if ( w_context.device )
			{
				w_context.device->Release( );
				w_context.device = nullptr;
			}
		}

	public:
		auto create( const std::string, const ImVec2, context_t &, const std::function<void( context_t & )> ) -> bool;
		auto render( context_t &, const ImVec2, const std::function<void( context_t &, const ImVec2 )> ) -> void;

		c_window( ) = default;
		~c_window( ) = default;
	};

	class c_render
	{
	private:
		inline static ImFont *roboto, *roboto_bd;

	public:
		c_render( ) = default;
		~c_render( ) = default;

		static auto menu_style( context_t & ) -> void;
		static auto menu_panel( context_t &, const ImVec2 ) -> void;
	};
}

#endif