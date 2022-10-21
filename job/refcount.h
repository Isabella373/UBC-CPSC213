#pragma once

void* rc_malloc   (int nbytes);
void  rc_keep_ref (void* p);
void  rc_free_ref (void* p);
