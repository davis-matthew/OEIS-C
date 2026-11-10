/*
 * A391500  a(n) is the number of ways to decompose A152242(n) into primes.
 * https://oeis.org/A391500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search: a(n) = number of ways to split the decimal digits of
 *            A152242(n) into primes (no leading zeros), counted by DP over
 *            digit positions
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9312 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t A391500_count(uint64_t m) {
    int digits[32];
    int len = 0;
    uint64_t t = m;
    while (t > 0) {
        digits[len++] = (int)(t % 10);
        t /= 10;
    }
    for (int i = 0; i < len / 2; ++i) {
        int x = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = x;
    }
    uint64_t ways[33];
    ways[len] = 1;
    for (int pos = len - 1; pos >= 0; --pos) {
        uint64_t w = 0;
        if (digits[pos] != 0) {
            uint64_t sub = 0;
            for (int j = pos + 1; j <= len; ++j) {
                sub = sub * 10 + (uint64_t)digits[j - 1];
                if (ways[j] != 0 && is_prime_u64(sub)) w += ways[j];
            }
        }
        ways[pos] = w;
    }
    return ways[0];
}

seq_t a(idx_t n) {
    static uint64_t *cache = NULL;
    static size_t cap = 0;
    static size_t cnt = 0;
    static uint64_t cand = 2;
    while (cnt < (size_t)n) {
        uint64_t c;
        for (;;) {
            c = A391500_count(cand);
            cand++;
            if (c != 0) break;
        }
        if (cnt >= cap) {
            size_t nc = cap ? cap * 2 : 256;
            uint64_t *tmp = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cap = nc;
        }
        cache[cnt++] = c;
    }
    return (seq_t)cache[n - 1];
}
