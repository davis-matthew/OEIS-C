/*
 * A247827  Least prime factor of A247676.
 * https://oeis.org/A247827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime factor of A247676(n), where A247676 lists the
 *            odd composites == 2 (mod 9), i.e. the composites of the form
 *            18t+11. Computed by search with trial division, cached
 * Range:     u8 n <= 5, u16 n <= 3548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9733 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *lpf_cache = NULL;
static size_t lpf_cap = 0;
static idx_t lpf_len = 0;
static idx_t next_t = 0;

/* returns least prime factor of odd cand, or 0 if cand is prime */
static uint64_t A247827_lpf_if_composite(seq_t cand)
{
    seq_t d;
    for (d = 3; d * d <= cand; d += 2) {
        if (cand % d == 0) return (uint64_t)(long long)d;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (lpf_len < n) {
        if ((size_t)lpf_len == lpf_cap) {
            size_t nc = lpf_cap ? lpf_cap * 2 : 64;
            uint64_t *p = (uint64_t *)realloc(lpf_cache, nc * sizeof(uint64_t));
            if (!p) return (seq_t)0;
            lpf_cache = p;
            lpf_cap = nc;
        }
        seq_t cand = (seq_t)18 * (seq_t)next_t + (seq_t)11;
        uint64_t f = A247827_lpf_if_composite(cand);
        if (f != 0) {
            lpf_cache[lpf_len++] = f;
        }
        ++next_t;
    }
    return (seq_t)lpf_cache[n - 1];
}
