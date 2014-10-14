/*
 * Copyright (c) 2014 Juniper Networks, Inc. All rights reserved.
 */

#ifndef vnsw_agent_diag_trace_route_hpp
#define vnsw_agent_diag_trace_route_hpp

#include "diag/diag.h"
#include "diag/diag_types.h"
#include "pkt/control_interface.h"

class DiagTable;
class TraceRoute: public DiagEntry {
public:
    static const int kMaxTtl = 16;
    static const int kMaxAttempts = 1;
    static const int kTimeout = 2000;
    static const int kBufferSize = 1024;

    TraceRoute(const TraceRouteReq *req, DiagTable *diag_table);
    virtual ~TraceRoute();

    virtual void SendRequest();
    void RequestTimedOut(uint32_t seqno);
    virtual void HandleReply(DiagPktHandler *handler);
    void ReplyLocalHop();
    void SendSummary();
    virtual bool IsDone();

    static void HandleRequest(DiagPktHandler *);

private:
    void FillHeader(AgentDiagPktData *data);
    void IncrementTtl();

    bool       done_;
    uint8_t    ttl_;  // ttl value set in the msg sent out
    uint16_t   max_attempts_; // max attempts for a specific ttl
    uint16_t   max_ttl_;      // max ttl upto which requests are sent
    std::string context_;
};

#endif
