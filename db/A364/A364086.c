/*
 * A364086  Fixed points of A002326, i.e., numbers k such that A002326(k) = k.
 * https://oeis.org/A364086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th fixed point of A002326, i.e. k with ord(2 mod 2k+1)
 *            = k; found by search over primes p=2k+1 (p = 7 or 1 mod 8) with
 *            ord_p(2) = (p-1)/2, as (A115591(n)-1)/2
 * Range:     u8 n <= 29, u16 n <= 3467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9732 (2026-11-10)
 * Helpers:   is_prime_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/powmod_u64.h"

static uint64_t *fp_cache = NULL;
static size_t fp_len = 0, fp_cap = 0;
static uint64_t fp_next_m = 3;

static bool fp_test(uint64_t p) {
    uint64_t e = (p - 1) / 2;
    if (powmod_u64(2, e, p) != 1) return false;
    uint64_t t = e;
    for (uint64_t q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            if (powmod_u64(2, e / q, p) == 1) return false;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1 && powmod_u64(2, e / t, p) == 1) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (fp_len < (size_t)n) {
        uint64_t m = fp_next_m;
        fp_next_m += 2;
        uint64_t r = m & 7;
        if (r != 1 && r != 7) continue;
        if (!is_prime_u64(m)) continue;
        if (!fp_test(m)) continue;
        if (fp_len == fp_cap) {
            size_t nc = fp_cap ? fp_cap * 2 : 64;
            uint64_t *tmp = (uint64_t *)realloc(fp_cache, nc * sizeof(uint64_t));
            if (!tmp) exit(1);
            fp_cache = tmp;
            fp_cap = nc;
        }
        fp_cache[fp_len++] = (m - 1) / 2;
    }
    return (seq_t)fp_cache[n - 1];
}
