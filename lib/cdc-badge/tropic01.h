#ifndef TROPIC01_H
#define TROPIC01_H  

// Initialize TROPIC01 secure element
// Returns true on success, false on failure
bool tropic01_init();
bool tropic01_secure_session_start();

#endif // TROPIC01_H    
