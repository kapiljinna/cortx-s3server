/*
 * Copyright (c) 2020 Seagate Technology LLC and/or its Affiliates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For any questions about this software or licensing,
 * please email opensource@seagate.com or cortx-questions@seagate.com.
 *
 */

#pragma once

#ifndef __S3_SERVER_AUDIT_INFO_H__
#define __S3_SERVER_AUDIT_INFO_H__

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#define REQUEST_TIME_SIZE 50
typedef unsigned long long UInt64;

enum class AuditFormatType {
  JSON,       // Logs audit information in Json format
  S3_FORMAT,  // Logs in Amazon s3 access log format
              // https://docs.aws.amazon.com/AmazonS3/latest/dev/LogFormat.html
};

std::string audit_format_type_to_string(enum AuditFormatType type);

class S3AuditInfo {
 private:
  // S3 Audit log format :
  // https://docs.aws.amazon.com/AmazonS3/latest/dev/LogFormat.html

  std::string bucket_owner_canonical_id;  // The canonical user ID of the owner
                                          // of the source
                                          // bucket.
  std::string bucket;  // The name of the bucket that the request was processed
                       // against.
  std::string time_of_request_arrival;  // The time at which the request was
                                        // received.
  std::string remote_ip;   // The apparent internet address of s3 client.
  std::string requester;   // The canonical user ID of the requester
  std::string request_id;  // A string generated by S3 to uniquely identify each
                           // request.
  std::string operation;   // The operation listed here is declared as
                           // REST.HTTP_method.resource_type.
  std::string object_key;  // The "key" part of the request
  std::string request_uri;  // The Request-URI part of the HTTP request message.
  int http_status;          // The numeric HTTP status code of the response.
  std::string error_code;   // The S3 Error Code, or "-" if no error occurred.
  size_t bytes_sent;        // The number of response bytes sent
  size_t object_size;       // The total size of the object in question.
  size_t bytes_received;    // The number of request bytes received by server.
  size_t total_time;  // The number of milliseconds the request was in flight
                      // from the server's perspective.
  size_t turn_around_time;  // The number of milliseconds that S3 spent
                            // processing your request.
  std::string referrer;    // The value of the HTTP Referrer header, if present.
                           // HTTP user-agents
  std::string user_agent;  // The value of the HTTP User-Agent header.
  std::string version_id;  // The version ID in the request
  std::string host_id;     // The x-amz-id-2 or S3 extended request ID.
  std::string signature_version;  // The signature version, SigV2 or SigV4, that
                                  // was used to authenticate the request.
  std::string cipher_suite;  // The Secure Sockets Layer (SSL) cipher that was
                             // negotiated for HTTPS request.
  std::string authentication_type;  // The type of request authentication used,
                                    // AuthHeader for authentication headers,
                                    // QueryString for query string.
  std::string host_header;          // The endpoint used to connect to S3

  bool publish_log = true;  // Flag to disable audit log for healthchecks.

 public:
  S3AuditInfo();
  // Setter methods for Audit Logger
  void set_bucket_owner_canonical_id(const std::string& bucket_owner_str);
  void set_bucket_name(const std::string& bucket_str);
  void set_time_of_request_arrival();
  void set_remote_ip(const std::string& remote_ip_str);
  void set_requester(const std::string& requester_str);
  void set_request_id(const std::string& request_id_str);
  void set_operation(const std::string& operation_str);
  void set_object_key(const std::string& key_str);
  void set_request_uri(const std::string& request_uri_str);
  void set_http_status(int httpstatus);
  void set_error_code(const std::string& error_code);
  void set_bytes_sent(size_t total_bytes_sent);
  void set_object_size(size_t obj_size);
  void set_bytes_received(size_t total_bytes_received);
  void set_total_time(size_t total_request_time);
  void set_turn_around_time(size_t request_turn_around_time);
  void set_referrer(const std::string& referrer_str);
  void set_user_agent(const std::string& user_agent_str);
  void set_version_id(const std::string& version_id_str);
  void set_host_id(const std::string& host_id_str);
  void set_signature_version(const std::string& authorization);
  void set_cipher_suite(const std::string& cipher_suite_str);
  void set_authentication_type(const std::string& authentication_type_str);
  void set_host_header(const std::string& host_header_str);
  void set_publish_flag(bool publish_flag);
  bool get_publish_flag();
  const std::string to_string();
  UInt64 convert_to_unsigned(size_t audit_member);
};

#endif
