# sf-inter-plugin-msgs

Inter-plugin messaging API for aircraft and addon developers to communicate with the [Shared Flight](https://sharedflight.com) plugin in X-Plane.

This allows third-party aircraft plugins to sync custom state (autosave data, aircraft-specific systems) between instances during a Shared Flight session.

## Overview

Shared Flight handles syncing of standard X-Plane datarefs and commands automatically via config files. However, some aircraft have custom internal state (e.g. FMS data, custom systems managed by SASL or C plugins) that can't be synced through datarefs alone.

This API provides two mechanisms for aircraft developers:

1. **Autosave State Sync** — When a new crew member joins a flight, their aircraft can receive the current state from the existing crew member. Think of it like loading a saved state.
2. **Live Aircraft Data** — Send arbitrary data between all instances of the aircraft in real-time during the flight.

## Quick Start

### 1. Include the header

Copy `includes/SharedFlightMessaging.h` into your project and include it:

```c
#include "SharedFlightMessaging.h"
```

### 2. Get the Shared Flight interface

During your flight loop, find the Shared Flight plugin and request its interface:

```c
static XPLMPluginID sfPluginID = XPLM_NO_PLUGIN_ID;
static SharedFlightInterPluginInterface sfInterface;

// In your flight loop callback:
if (sfPluginID == XPLM_NO_PLUGIN_ID) {
    sfPluginID = XPLMFindPluginBySignature(XPLM_SHARED_FLIGHT_PLUGIN_SIG);
    if (sfPluginID != XPLM_NO_PLUGIN_ID) {
        XPLMSendMessageToPlugin(sfPluginID,
            MSG_CMD_GET_SHARED_FLIGHT_INTERFACE, &sfInterface);
    }
}
```

### 3. Use the interface

Once you have the interface, you can register callbacks and send data:

```c
// Register a callback for when Shared Flight requests your autosave state
sfInterface.SetAutosaveRequestCallback(myAutosaveRequestHandler);

// Register a callback for when autosave state is received from another crew member
sfInterface.SetAutosaveLoadCallback(myAutosaveLoadHandler);

// Register a callback for live aircraft data from other instances
sfInterface.SetAircraftDataCallback(myAircraftDataHandler);

// Send autosave state to Shared Flight for relay to joining crew
sfInterface.TransmitAutosaveState(data, dataSize);

// Tell Shared Flight you're done applying autosave state
sfInterface.AutosaveStateApplyDone();

// Send live data to all other instances
sfInterface.TransmitAircraftData(data, dataSize);
```

## API Reference

### Interface Functions

| Function | Description |
|----------|-------------|
| `SetAutosaveRequestCallback(cb)` | Register a callback that Shared Flight calls when it needs your current autosave/state data to send to a joining crew member. |
| `TransmitAutosaveState(data, size)` | Send your autosave state data to Shared Flight. Call this from within your autosave request callback. |
| `SetAutosaveLoadCallback(cb)` | Register a callback that receives autosave state data from the existing crew member when you join a flight. |
| `AutosaveStateApplyDone()` | Notify Shared Flight that you have finished applying the received autosave state. |
| `TransmitAircraftData(data, size)` | Send arbitrary data to all other instances of your aircraft in the flight. |
| `SetAircraftDataCallback(cb)` | Register a callback that receives live aircraft data from other instances. |

### Callback Signatures

```c
// Called when Shared Flight needs your autosave state
typedef void (*SharedFlightAutosaveRequestCallback)();

// Called with autosave data to load (when joining a flight)
typedef void (*SharedFlightAutosaveLoadCallback)(const void *data, size_t size);

// Called with live aircraft data from other instances
typedef void (*SharedFlightAircraftDataCallback)(const void *data, size_t size);
```

### Message IDs

These are used with `XPLMSendMessageToPlugin` for the lower-level message-based API:

| Constant | Value | Direction | Description |
|----------|-------|-----------|-------------|
| `MSG_CMD_GET_SHARED_FLIGHT_INTERFACE` | `0x085346FF` | To SF | Request the function interface struct |
| `MSG_CMD_ACF_REQST_AUTOSAVE_STATE` | `0x08534600` | SF to Aircraft | Request autosave state |
| `MSG_CMD_SF_TRANSMIT_AUTOSAVE_STATE` | `0x08534601` | Aircraft to SF | Send autosave state |
| `MSG_CMD_ACF_LOAD_AUTOSAVE_STATE` | `0x08534603` | SF to Aircraft | Apply received autosave state |
| `MSG_MSG_SF_AUTOSAVE_STATE_DONE` | `0x88534600` | Aircraft to SF | Autosave state applied |
| `MSG_CMD_SF_TRANSMIT_AIRCRAFT_DATA` | `0x08534605` | Aircraft to SF | Send live data to others |
| `MSG_CMD_ACF_NEW_AIRCRAFT_DATA` | `0x08534607` | SF to Aircraft | Receive live data from others |

### Data Format

Data is passed as a `SF_MSG_DATA` struct (alias for `SASL_MSG_StringData`):

```c
typedef struct {
    size_t mSize;        // Size of the data in bytes
    const char *mData;   // Pointer to the data
} SF_MSG_DATA;
```

The data content is opaque to Shared Flight — you can use any serialization format (JSON, binary, etc.).

## Examples

### C/C++ Plugin

See [`examples/c-sample-plugin/`](examples/c-sample-plugin/) for a complete working example of a C++ X-Plane plugin that uses the function interface API.

**Build:**
```bash
cd examples/c-sample-plugin
mkdir build && cd build
cmake .. && make
```

### SASL Plugin

See [`examples/SASL/`](examples/SASL/) for a SASL-based example using Lua message passing via `sasl.sendMessageToPlugin`.

## Plugin Signature

When finding the Shared Flight plugin, use:

```c
#define XPLM_SHARED_FLIGHT_PLUGIN_SIG "justinsnapp.SharedFlight"
```

This is defined in `SharedFlightMessaging.h`.

## License

See [LICENSE](LICENSE) for details.
