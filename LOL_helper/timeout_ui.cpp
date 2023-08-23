#include	"timeout_ui.h"
const std::wstring Notify_Ui_Base::kClassName = L"Basic";
Notify_Ui_Base::Notify_Ui_Base(int all_tick):all_tick(all_tick)
{
	
}

Notify_Ui_Base::~Notify_Ui_Base()
{
}

std::wstring Notify_Ui_Base::GetSkinFolder()
{
	return L"basic";
}

std::wstring Notify_Ui_Base::GetSkinFile()
{
	return L"trans.xml";
}

std::wstring Notify_Ui_Base::GetWindowClassName() const
{
	return kClassName;
}

void Notify_Ui_Base::InitWindow()
{
	btn = dynamic_cast<ui::Button*>(FindControl(L"testbtn"));
	btn->AttachClick(std::bind(&Notify_Ui_Base::Clicked, this, std::placeholders::_1));
	_thread = std::thread(&Notify_Ui_Base::startTimer, this);
}


LRESULT Notify_Ui_Base::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}
bool	Notify_Ui_Base::Clicked(ui::EventArgs* args) {
	_thread_status = false;
	nbase::ThreadManager::PostTask(kThreadNetwork, []() {
		raw_lcu_api::getInstance().decline_game();
		});
	this->Close(1);
	return	true;
}
void Notify_Ui_Base::startTimer() {
	int	n = 0;
	while (_thread_status&&n<all_tick)
	{
		n += 100;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (_thread_status)
	{
		raw_lcu_api::getInstance().accept_game();
		this->Close(1);
	}
	_thread.detach();
}