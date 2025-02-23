#include "Application_Layer.h"
#include "pb_encode.h"
#include "test.pb.h"
#include "pb_decode.h"
#define BUFFER_SIZE 128




bool bkit_send_message(const sensor_data_t* data) {
    uint8_t buffer[BUFFER_SIZE];
    size_t message_length;

    // Chuyển dữ liệu vào struct Protobuf
    TestMessage sensor_pb = {
        .id = data->id,
        .temperature = data->temperature,
        .humidity = data->humidity,
        .timestamp = data->timestamp
    };

    // Mã hóa Protobuf
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, BUFFER_SIZE);
    if (!pb_encode(&stream, TestMessage_fields, &sensor_pb)) {
        DEBUG_PRINT("Protobuf encode failed!");
        return false;
    }
    message_length = stream.bytes_written;

    // Tạo header BKIT-COM
    bkit_header_t header = {
        .sync = 0xA5,
        .length = message_length,
        .checksum = bkit_compute_checksum(buffer, message_length)
    };

    // Gửi dữ liệu qua UART
    if (!message_processing_send((uint8_t*)&header, sizeof(header))) {
        DEBUG_PRINT("Message processing send header failed!");
        return false;
    }
    if (!message_processing_send(buffer, message_length)) {
        DEBUG_PRINT("Message processing send payload failed!");
        return false;
    }

    return true;
}


bool bkit_receive_message(sensor_data_t* data) {
    uint8_t buffer[BUFFER_SIZE];
    bkit_header_t header;

    // Nhận header từ Message Processing Layer
    if (!message_processing_receive((uint8_t*)&header, sizeof(header))) {
        DEBUG_PRINT("Header receive failed!");
        return false;
    }

    // Kiểm tra byte đồng bộ
    if (header.sync != 0xA5) {
        DEBUG_PRINT("Sync byte mismatch!");
        return false;
    }

    // Nhận payload
    if (uart_receive(buffer, header.length) != header.length) {
        DEBUG_PRINT("Payload receive failed!");
        return false;
    }

    // Kiểm tra checksum
    if (header.checksum != bkit_compute_checksum(buffer, header.length)) {
        DEBUG_PRINT("Checksum mismatch!");
        return false;
    }

    // Giải mã Protobuf
    pb_istream_t stream = pb_istream_from_buffer(buffer, header.length);
    TestMessage sensor_pb = {0};

    if (!pb_decode(&stream, TestMessage_fields, &sensor_pb)) {
        DEBUG_PRINT("Protobuf decode failed!");
        return false;
    }

    // Chuyển dữ liệu về struct ứng dụng
    data->id = sensor_pb.id;
    data->temperature = sensor_pb.temperature;
    data->humidity = sensor_pb.humidity;
    data->timestamp = sensor_pb.timestamp;

    return true;
}

