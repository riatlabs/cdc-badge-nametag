#ifndef TCA9535_H
#define TCA9535_H

// Initialize pin expander (TCA9535)
// Returns true on success, false on failure
bool pin_expander_init(void);
char pin_expander_process_irq();

#endif // TCA9535_H