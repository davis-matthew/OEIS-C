/*
 * A248773  Greatest 7th power integer that divides n!.
 * https://oeis.org/A248773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏{p prime ≤ n} p^( 7 * ⌊ v_p(n!) / 7 ⌋ ) where v_p(n!) =
 *            Σ_{k≥1} ⌊ n / p^k ⌋ This gives the greatest seventh‑power
 *            integer that divides n!
 * Range:     u8 n <= 15, u16 n <= 17, u32 n <= 23, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

/* Cache of primes up to the largest n seen */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Ensure that the global array `primes` contains all primes ≤ n */
static void _ensure_primes_upto(idx_t n)
{
    if (n <= primes_limit)
        return;
    free(primes);
    build_primes_upto(n, &primes, &primes_cnt);
    primes_limit = n;
}

/* Greatest 7th‑power integer that divides n! */
seq_t a(idx_t n)
{
    if (n < 2)         /* a(1) = 1, and the same holds for n = 0 (unused) */
        return (seq_t)1;

    _ensure_primes_upto(n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if ((idx_t)p > n)
            break;

        /* Compute v_p(n!) using Legendre's formula */
        idx_t t = n;
        idx_t vp = 0;
        while (t)
        {
            t /= (idx_t)p;
            vp += t;
        }

        /* Largest multiple of 7 not exceeding vp */
        idx_t expo = (vp / 7) * 7;
        if (expo == 0)
            continue;

        /* Multiply result by p, expo times (i.e. by p^expo) */
        for (idx_t k = 0; k < expo; ++k)
            result = seq_mul(result, (seq_t)p);
    }

    return result;
}
