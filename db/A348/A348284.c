/*
 * A348284  Numbers k such that k | k" where k" is the 2nd arithmetic
 *          derivative of k.
 * https://oeis.org/A348284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer k such that k divides its
 *            second arithmetic derivative k''. The implementation enumerates
 *            integers incrementally, keeps those that satisfy the condition,
 *            and memoises them. For computing the arithmetic derivative we
 *            use the classic formula k' = Σ e_i * (k / p_i)...
 * Range:     u8 n <= 37, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   ensure_res_cap, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ensure_res_cap.h"

static seq_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1)   /* 1' = 0, (0)' is defined as 0 */
        return 0;

    /* A 64‑bit integer has at most 12 distinct prime factors;
       a 64-element buffer is more than enough. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * (seq_t)(n / primes[i]); /* e * k/p */
        sum += term;
    }
    return sum;
}

/* Main sequencedite function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive, not used by OEIS. */

    /* static state: cache of accepted numbers and range of checked k's */
    static seq_t  *cache = NULL;     /* store the sequence values */
    static size_t cache_len = 0;     /* how many values are stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static uint64_t checked = 0;     /* largest integer examined so far */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        ++checked;

        /* Compute the first and second arithmetic derivatives */
        seq_t d1 = arithmetic_derivative(checked);
        seq_t d2 = arithmetic_derivative((uint64_t)d1);

        bool ok;
        if (d2 == 0) {
            ok = true;                     /* k | 0 holds for any k */
        } else {
            ok = (d2 % (seq_t)checked) == 0;
        }

        if (ok) {
            ensure_res_cap(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = (seq_t)checked;
        }
    }
    return cache[n - 1];
}
