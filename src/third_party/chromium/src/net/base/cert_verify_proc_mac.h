// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_CERT_VERIFY_PROC_MAC_H_
#define NET_BASE_CERT_VERIFY_PROC_MAC_H_

#include "net/base/cert_verify_proc.h"

#include "base/synchronization/lock.h"

namespace net {

// Performs certificate path construction and validation using OS X's
// Security.framework.
class CertVerifyProcMac : public CertVerifyProc {
 public:
  CertVerifyProcMac();

 protected:
  virtual ~CertVerifyProcMac();

 private:
  virtual int VerifyInternal(X509Certificate* cert,
                             const std::string& hostname,
                             int flags,
                             CRLSet* crl_set,
                             CertVerifyResult* verify_result) OVERRIDE;

  // Blocks multiple threads from verifying the cert simultaneously.
  // (Marked mutable because it's used in a const method.)
  mutable base::Lock verification_lock_;
};

}  // namespace net

#endif  // NET_BASE_CERT_VERIFY_PROC_MAC_H_
