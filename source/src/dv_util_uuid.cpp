#include "dv_utilities/dv_util_uuid.hpp"

#include <uuid.h>
#include <spookyhash.hpp>

using namespace dvsku;

uuid dv_util_uuid::create() {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    uuids::uuid_random_generator gen{ generator };
    
    auto uuid = gen().as_bytes();
    return SpookyHash::Hash64(uuid.data(), uuid.size(), 0xdeadbeef);
}

uuid dv_util_uuid::create(const std::string& str) {
    return SpookyHash::Hash64(str.data(), str.size(), 0xdeadbeef);
}
