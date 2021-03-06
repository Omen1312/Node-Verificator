#include "transaction.h"
#include <meta_log.hpp>
#include <open_ssl_decor.h>

bool RejectedTXInfo::parse(std::string_view tx_sw)
{
    uint64_t index = 0;

    {
        const uint64_t hash_size = 32;

        if (index + hash_size >= tx_sw.size()) {
            DEBUG_COUT("corrupt hash size");
            return false;
        }

        std::copy_n(tx_sw.begin(), hash_size, tx_hash.begin());
        index += hash_size;
    }

    {
        std::string_view varint_arr(&tx_sw[index], tx_sw.size() - index);
        uint64_t reason_varint_size = read_varint(reason, varint_arr);
        if (reason_varint_size < 1) {
            DEBUG_COUT("corrupt varint size");
            return false;
        }
        index += reason_varint_size;
    }

    return (index == tx_sw.size());
}

bool RejectedTXInfo::make(const sha256_2& tx_hash, uint64_t reason)
{
    std::vector<char> tx_raw;
    tx_raw.insert(tx_raw.end(), tx_hash.begin(), tx_hash.end());
    append_varint(tx_raw, reason);
    std::string_view record_raw_sw(tx_raw.data(), tx_raw.size());

    return parse(record_raw_sw);
}
