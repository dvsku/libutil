#include "libutil/uuid/uuid.hpp"

#include <uuid.h>
#include <spookyhash.hpp>

libutil::uuid_t libutil::uuid::create(unique_uuid_pred_t pred) {
    static std::random_device                        random_device = {};
    static std::array<int, std::mt19937::state_size> seed          = {};
    uuid_t                                           uuid          = 0;

    do {
        std::generate(std::begin(seed), std::end(seed), std::ref(random_device));

        auto seed_seq   = std::seed_seq(std::begin(seed), std::end(seed));
        auto engine     = std::mt19937(seed_seq);
        auto random_gen = uuids::uuid_random_generator(engine);
        auto generated  = random_gen().as_bytes();

        uuid = create(generated.data(), generated.size());
    } while (pred && !pred(uuid));

    return uuid;
}

libutil::uuid_t libutil::uuid::create(const std::string& str) {
    return create(str.data(), str.size());
}

libutil::uuid_t libutil::uuid::create(const void* data, size_t size) {
    return SpookyHash::Hash64(data, size, 0xdeadbeef);
}
