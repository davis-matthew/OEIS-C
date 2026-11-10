/*
 * A342958  a(n) is the least prime that starts a string of exactly n primes
 *          p_1, p_2, ... p_n where p_{i+1} = p_i-A003132(p_i), but
 *          p_n-A003132(p_n) is not prime.
 * https://oeis.org/A342958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = least prime p such that the chain p_1=p,
 *            p_{i+1}=p_i-A003132(p_i) has exactly n primes in a row; found by
 *            sieving with a doubling limit and a DP on chain length
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t A003132_ss(uint64_t x) {
    uint64_t r = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        r += d * d;
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t limit = 1u << 16;
    const uint64_t cap = 1ull << 31;
    if (n == 0) return (seq_t)0;
    while (limit <= cap) {
        unsigned char *comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
        unsigned char *len = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!comp || !len) {
            free(comp);
            free(len);
            return (seq_t)0;
        }
        comp[0] = 1;
        comp[1] = 1;
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (!comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
        uint64_t found = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (comp[i]) continue;
            uint64_t s = A003132_ss(i);
            unsigned char l = 1;
            if (i > s) {
                uint64_t d = i - s;
                if (d >= 2 && !comp[d]) {
                    unsigned v = (unsigned)len[d] + 1;
                    l = (unsigned char)(v > 255 ? 255 : v);
                }
            }
            len[i] = l;
            if ((idx_t)l == n) {
                found = i;
                break;
            }
        }
        free(comp);
        free(len);
        if (found) return (seq_t)found;
        limit <<= 1;
    }
    return (seq_t)0;
}
