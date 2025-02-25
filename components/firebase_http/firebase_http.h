#ifndef FIREBASE_HTTP_H
#define FIREBASE_HTTP_H

#include "firebase_config.h"

void firebase_send_data(const char *topic, float temperature, float humidity);
void firebase_fetch_data(const char *topic);

#endif // FIREBASE_HTTP_H
