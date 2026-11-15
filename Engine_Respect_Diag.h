#ifndef __ENGINE_RESPECT_DIAG_H
#define __ENGINE_RESPECT_DIAG_H

#include "rayanmessage.h"
#include "Algorithm.h"

class Engine_Respect_Diag : public BaseAlgorithm
{
public:
	virtual bool InternalSwitchCase(CommandAdorner* Command);
	const static u16 Class_ID;
	Engine_Respect_Diag(void);
	~Engine_Respect_Diag(void);

	bool EnginDTC(CommandAdorner* Command);
	bool VVTPosSelf_Learn(CommandAdorner* Command);
	bool ReplaceVGT(CommandAdorner* Command);
	bool ReplaceECU(CommandAdorner* Command);
	bool Keyboard_Actuator(CommandAdorner* Command);
	bool Delay_Actuator(CommandAdorner* Command);
	bool Error_Handlear();
private:
	bool delay_exitsuported(u32 delay, char* msg, bool show_timer);
	
};
#endif //__Engine_Respect_Diag_H
