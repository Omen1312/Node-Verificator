#ifndef CHAIN_H
#define CHAIN_H

#include "block.h"
#include "wallet.h"

#include <atomic>

class BlockChain {
private:
    struct ProxyStat {
        std::map<std::string, std::vector<uint64_t>> proxy_rps;
        uint64_t proxy_statistics_count;
        std::map<std::string, std::vector<uint64_t>> torrent_ping;
        uint64_t torrent_statistics_count;
    };

    const std::map<std::string, std::string> test_nodes = {
        { "0x00b888869e8d4a193e80c59f923fe9f93fd6552875c857edbe", "us" },
        { "0x00bc4787973cb36f47d4f274bc340cb3e1402030955c85e563", "cn" },
        { "0x00cacf8f42f4ffa95bc4a5eea3cf5986f56e13eed8ae012a67", "eu" }
    };

    sha256_2 prev_hash = { { 0 } };
    uint64_t state_hash_xx64 = 0;

    WalletMap wallet_map;

    std::map<std::string, uint64_t> node_state;
    std::map<std::string, ProxyStat> proxy_statistics;

    std::vector<TX*> statistics_tx_list;

    std::atomic<std::map<std::string, std::pair<int, int>>*> wallet_statistics = nullptr;
    std::atomic<std::deque<std::pair<std::string, uint64_t>>*> wallet_request_addreses = nullptr;

public:
    BlockChain();

    bool can_apply_block(Block* block);

    bool apply_block(Block* block);

    Block* make_forging_block(uint64_t timestamp);

    Block* make_state_block(uint64_t timestamp);

    Block* make_common_block(uint64_t timestamp, std::vector<TX*>& transactions);

    Block* make_statistics_block(uint64_t timestamp);

    std::atomic<std::map<std::string, std::pair<int, int>>*>& get_wallet_statistics();
    std::atomic<std::deque<std::pair<std::string, uint64_t>>*>& get_wallet_request_addreses();

private:
    Block* make_block(uint64_t b_type, uint64_t b_time, sha256_2 prev_b_hash, std::vector<char>& tx_buff);

    uint64_t get_fee(uint64_t cnt);

    bool try_apply_block(Block* block, bool apply);
    bool can_apply_old_block(Block* block);
    bool can_apply_common_block(Block* block);
    bool can_apply_state_block(Block* block, bool);
    bool can_apply_forging_block(Block* block);
};

#endif // CHAIN_H
