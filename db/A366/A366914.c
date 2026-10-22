/*
 * A366914  Numbers expressible as the sum of their distinct prime factors
 *          raised to a natural exponent.
 * https://oeis.org/A366914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers expressible as the sum of their distinct prime factors
 *            raised to natural exponents
 * Range:     u8 n <= 85, u16 n <= 6877, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   backtrack_rec, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* provides factor_u64() */
#include "../utils/backtrack_rec.h"

/* Check whether m satisfies the condition */
static bool canRepresent(uint64_t m)
{
    if (m < 2)
        return false;

    uint64_t primes[64], exponents[64];
    size_t cnt;

    factor_u64(m, primes, exponents, &cnt);
    if (cnt == 0)
        return false;

    unsigned __int128 minRem[65];
    minRem[cnt] = 0;
    for (int i = (int)cnt - 1; i >= 0; --i)
        minRem[i] = (unsigned __int128)primes[i] + minRem[i + 1];

    return backtrack_rec(primes, cnt, 0, (unsigned __int128)m, minRem);
}

/* Global cache for sequence values */
static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t seq_len   = 0;       /* how many terms stored */
static size_t seq_cap   = 0;       /* current array capacity */
static uint64_t next_val = 1;      /* last tested integer */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* base case: undefined, but harmless */

    /* Ensure cache contains at least n terms */
    while (seq_len < (size_t)n) {
        ++next_val;                 /* test next integer */

        if (canRepresent(next_val)) {
            if (seq_len == seq_cap) { /* grow array if needed */
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();         /* allocation failure */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)next_val;
        }
    }
    return seq_cache[n - 1];
}
