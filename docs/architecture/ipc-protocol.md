# Net-browser IPC Protocol (MVP)

## Goal
GUI (C#) ↔ Core (C++) lokal IPC:
- Navigate/Back/Forward/Reload/NewTab/CloseTab
- Event stream: UrlChanged, TitleChanged, LoadStateChanged, CrashDetected

## Encoding & framing
- Payload: protobuf (proto3)
- Frame: uint32 little-endian length prefix + N bytes payload
- Max frame: 16 MiB

## Transport
- Windows: Named Pipes
- Later: Unix domain sockets

## Handshake
ClientHello -> ServerHello (protocol_major/minor)

## Notes
- MVP uses a tiny text-based fallback too (for demo simplicity).
- For production: strict protobuf-only + ACL-hardening + multi-client.