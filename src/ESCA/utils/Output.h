#pragma once

//#define DEBUG_OUTPUT

#ifdef DEBUG_OUT
std::error_code errorCodeForDebug;
llvm::raw_fd_ostream debug_out("debug_info.txt", errorCodeForDebug);
#define Cout debug_out
#else
#define Cout llvm::nulls()
#endif