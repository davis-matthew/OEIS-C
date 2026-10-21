/*
 * A293576  Numbers n such that the set of exponents in expression for 2*n as
 *          a sum of distinct powers of 2 can be partitioned into two parts
 *          with equal sums.
 * https://oeis.org/A293576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 9977, u32 n <= 9977, u64 n <= 9977,
 *            u128 n <= 9977, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293576:  numbers n such that the multiset of exponents
 * in the binary representation of 2*n can be partitioned into two
 * subsets with equal sums.  The function generates the sequence by
 * incremental search and memoises found terms. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Test whether the exponent set of 2*n admits an equal‑sum partition. */
static bool can_partition(seq_t n)
{
    /* 0 is a valid term (empty exponent set). */
    if (n == 0) return true;

    seq_t m = n << 1;               /* 2*n */
    uint64_t pos[128];
    int cnt = 0;
    seq_t tmp = m;
    unsigned int exp = 0;

    /* Extract exponent positions of set bits in 2*n. */
    while (tmp) {
        if ((tmp & 1) != 0) {
            if (cnt >= 128) return false; /* safety limit */
            pos[cnt++] = exp;
        }
        tmp >>= 1;
        exp++;
    }

    /* Hamming weight 1 or 2 cannot be split. */
    if (cnt <= 2) return false;

    /* Sum of exponents. */
    unsigned long long sum = 0;
    for (int i = 0; i < cnt; ++i) sum += pos[i];
    if (sum & 1ULL) return false;           /* odd total cannot be split */

    unsigned long long target = sum / 2ULL;

    /* Boolean DP via bitset to test subset sum existence. */
    if (target <= 2047) {
        uint64_t bits[32] = {0};
        bits[0] = 1ULL; /* sum 0 achievable */
        for (int i = 0; i < cnt; ++i) {
            unsigned long long w = pos[i];
            if (w > target) continue;
            size_t ws = w / 64;
            unsigned int bs = w % 64;
            for (size_t j = (target / 64) + 1; j-- > 0; ) {
                uint64_t add = 0;
                if (j >= ws) {
                    add = bits[j - ws] << bs;
                    if (bs && j >= ws + 1)
                        add |= bits[j - ws - 1] >> (64 - bs);
                }
                bits[j] |= add;
            }
        }
        size_t idx = target / 64;
        return (bits[idx] & (1ULL << (target % 64))) != 0;
    } else {
        size_t words = target / 64 + 1;
        uint64_t *bits = (uint64_t*)calloc(words, sizeof(uint64_t));
        if (!bits) return false;
        bits[0] = 1ULL;
        for (int i = 0; i < cnt; ++i) {
            unsigned long long w = pos[i];
            if (w > target) continue;
            size_t ws = w / 64;
            unsigned int bs = w % 64;
            for (size_t j = words; j-- > 0; ) {
                uint64_t add = 0;
                if (j >= ws) {
                    add = bits[j - ws] << bs;
                    if (bs && j >= ws + 1)
                        add |= bits[j - ws - 1] >> (64 - bs);
                }
                bits[j] |= add;
            }
        }
        size_t idx = target / 64;
        bool ok = (bits[idx] & (1ULL << (target % 64))) != 0;
        free(bits);
        return ok;
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = 0;   /* next integer to test */

    if (n == 0) return 0;   /* safeguard – n should be >=1 */

    while (cache_len < n) {
        if (can_partition(next_candidate)) {
            /* Expand cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {        /* allocation failure – give up */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
