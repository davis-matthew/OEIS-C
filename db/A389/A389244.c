/*
 * A389244  Powerful part of n*(n+1): a(n) = A057521(n*(n+1)).
 * https://oeis.org/A389244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057521(n*(n+1))
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    seq_t res  = 1;

    /* Determine prime limit needed for factorisation */
    idx_t limit = (idx_t)floor_sqrt(prod);

    /* Static cache of primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t  prime_limit = 0;

    if (limit > prime_limit) {
        free(primes);               /* free old list if any */
        build_primes_upto(limit, &primes, &prime_cnt);
        prime_limit = limit;
    }

    /* Factorise prod and keep only prime powers with exponent ≥ 2 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        int exp = 0;
        while (prod % p == 0) {
            prod /= p;
            ++exp;
        }
        if (exp >= 2) {
            seq_t part = ipow_u64_idx(p, (idx_t)exp);
            res *= part;
        }
    }
    return res;
}
