/*
 * GPS_NEO_6M.h
 *
 *  Created on: Sep 10, 2020
 *      Author: odemki
 */

#ifndef INC_GPS_NEO_6M_H_
#define INC_GPS_NEO_6M_H_

//int nmea0183_checksum(char *msg);
void simple_read_stream_data_from_GPS (uint8_t *GPS_buff, int size_buff);
int nmea0183_checksum(char *msg);
void parsing_GPS(uint8_t *GPS_buff, int size_buff);

#endif /* INC_GPS_NEO_6M_H_ */
