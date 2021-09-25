# Gametable
*The deployable libmahjong game service*

Gametable is a sevice to host network-based, [libmahjong-backed](https://github.com/realliance/libmahjong) riichi mahjong matches.

## Dependencies

- `git`
- `cmake`
- pistache (AUR: `pistache-git`)
- `rapidjson`
- `spdlog`

## Getting Started

```
# Bootstrap build/
cmake -S . -B build

# Build executable
cmake --build build
```

## Building a Client for Gametable

### Option 1: Use Tenpai

[Tenpai](https://github.com/realliance/tenpai) is a boilerplate designed for easy libmahjong Player Controller development. Out of the box, Tenpai supports connecting to gametable matches as well as running local simulations to test your bot with a larger sample size. See the project for more details.

### Option 2: Use the Gametable SDK

The [Gametable SDK](https://github.com/realliance/gametable-sdk) can be included in your own project to join remote matches with your Player Controller.

**Example Connection**

```
#include <gametablesdk/network.h>

using GametableSDK::Network::connectToMatch;

int main() {
  MyBot botInstance;
  connectToMatch(botInstance, "http://gametable.example.com:8080");
  return 0;
}
```
