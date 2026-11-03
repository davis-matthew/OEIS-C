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
 * Validated: standard, pass, 78 terms through n = 9854 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Helper: backtracking search for exponents */
static bool backtrack_rec(const uint64_t *pr, size_t k, size_t idx,
                          unsigned __int128 rem,
                          const unsigned __int128 *minRem)
{
    if (rem < minRem[idx])
        return false;          /* minimal possible sum too large */

    if (idx == k)
        return rem == 0;       /* all primes processed */

    uint64_t p = pr[idx];
    unsigned __int128 minNext = minRem[idx + 1];

    unsigned __int128 val = p;          /* p^1 */
    while (1) {
        if (val > rem - minNext)
            break;

        if (backtrack_rec(pr, k, idx + 1, rem - val, minRem))
            return true;

        if (val > rem / p)   /* avoid overflow of val *= p */
            break;
        val *= p;            /* next exponent */
    }
    return false;
}

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
                seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
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
