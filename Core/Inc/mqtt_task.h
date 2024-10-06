/*
 * mqtt_task.h
 *
 *  Created on: 2020. 5. 19.
 *      Author: jihoo
 */

#ifndef MQTT_TASK_H_
#define MQTT_TASK_H_

#include "MQTTClient.h"
#include "MQTTInterface.h"
#include "cmsis_os.h"

#include <string.h>

#define MQTT_BUFSIZE	1024

void MqttClientSubTask(void const *argument);
void MqttClientPubTask(void const *argument);
int  MqttConnectBroker(void);
void MqttMessageArrived(MessageData* msg);

#endif /* MQTT_TASK_H_ */
