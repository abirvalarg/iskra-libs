#include "timer.h"
#include "RCC.h"

static void tim_bas_set_psc(void *hw, Word psc);
static void tim_bas_set_arr(void *hw, Word arr);
static void tim_bas_start(void *hw);
static void tim_bas_stop(void *hw);
static Word tim_bas_cfg(void *hw, struct TimerConfig cfg);
static bool tim_bas_reset(void *hw);
static void tim_bas_trigger(void *hw);

static const struct TimerVTable basTimVT = {
	.init = NULL,
	.uninit = NULL,
	.set_psc = tim_bas_set_psc,
	.set_arr = tim_bas_set_arr,
	.start = tim_bas_start,
	.stop = tim_bas_stop,
	.config = tim_bas_cfg,
	.reset = tim_bas_reset,
	.trigger = tim_bas_trigger
};

static struct TimerEtc TIM6_etc = {
	.on_update = NULL
};

static struct TimerEtc TIM7_etc = {
	.on_update = NULL
};

const struct Timer TIM6 = {
	.__vt = &basTimVT,
	.hw = &TIM6_BASE,
	.busEnr = &RCC_BASE->APB1ENR,
	.etc = &TIM6_etc,
	.type = Timer_Basic,
	.enrPos = 4
};

const struct Timer TIM7 = {
	.__vt = &basTimVT,
	.hw = &TIM7_BASE,
	.busEnr = &RCC_BASE->APB1ENR,
	.etc = &TIM7_etc,
	.type = Timer_Basic,
	.enrPos = 5
};

void Timer_enable(const struct Timer *tm, bool state)
{
	if(state)
	{
		*tm->busEnr |= (1 << tm->enrPos);
		void (*init)(void*) = tm->__vt->init;
		if(init)
			init(tm->hw);
	}
	else
	{
		void (*uninit)(void*) = tm->__vt->uninit;
		if(uninit)
			uninit(tm->hw);
		*tm->busEnr &= ~(1 << tm->enrPos);
	}
}

void Timer_set_time(const struct Timer *tm, Word psc, Word arr)
{
	typedef void (*func_t)(void*, Word);
	func_t psc_func = tm->__vt->set_psc,
		   arr_func = tm->__vt->set_arr;
	if(psc_func)
		psc_func(tm->hw, psc);
	if(arr_func)
		arr_func(tm->hw, arr);
}

Word Timer_config(const struct Timer *tm, struct TimerConfig cfg)
{
	Word (*func)(void*, struct TimerConfig) = tm->__vt->config;
	if(func)
		return func(tm->hw, cfg);
	else
		return 0;
}

void Timer_start(const struct Timer *tm)
{
	void (*func)(void*) = tm->__vt->start;
	if(func)
		func(tm->hw);
}

void Timer_stop(const struct Timer *tm)
{
	void (*func)(void*) = tm->__vt->stop;
	if(func)
		func(tm->hw);
}

bool Timer_reset_int(const struct Timer *tm)
{
	bool (*func)(void*) = tm->__vt->reset;
	if(func)
		return func(tm->hw);
	else
		return false;
}

void Timer_trigger(const struct Timer *tm)
{
	void (*func)(void*) = tm->__vt->trigger;
	if(func)
		func(tm->hw);
}

TimerHandler Timer_on_update(const struct Timer *tm, TimerHandler func)
{
	TimerHandler old_func = tm->etc->on_update;
	tm->etc->on_update = func;
	return old_func;
}

static void tim_bas_set_psc(void *_hw, Word psc)
{
	struct BasicTimerReg *hw = _hw;
	hw->PSC = psc & 0xffff;
}

static void tim_bas_set_arr(void *_hw, Word arr)
{
	struct BasicTimerReg *hw = _hw;
	hw->ARR = arr & 0xffff;
}

static void tim_bas_start(void *_hw)
{
	struct BasicTimerReg *hw = _hw;
	hw->CR1 |= 1;
}

static void tim_bas_stop(void *_hw)
{
	struct BasicTimerReg *hw = _hw;
	hw->CR1 &= ~1;
}

static Word tim_bas_cfg(void *_hw, struct TimerConfig cfg)
{
	struct BasicTimerReg *hw = _hw;
	bool state = hw->CR1 & 1;
	hw->CR1 = cfg.preload << 7 | cfg.onePulse << 3 | cfg.counterOnly << 2 | cfg.updateDisable << 1 | state;
	hw->CR2 = cfg.masterMode << 4;
	hw->DIER = cfg.DMARequest << 8 | cfg.interrupt;
	return 1;
}

static bool tim_bas_reset(void *_hw)
{
	struct BasicTimerReg *hw = _hw;
	bool res = hw->SR;
	hw->SR = 0;
	return res;
}

static void tim_bas_trigger(void *_hw)
{
	struct BasicTimerReg *hw = _hw;
	hw->EGR = 1;
}
