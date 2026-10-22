/*
 * A353931  Least run-sum of the prime indices of n.
 * https://oeis.org/A353931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over prime factors p^e of n of (index(p) * e), where
 *            index(p) = pi(p-1)+1 and pi(x) is the prime counting function.
 *            For n = 1 the value is 0
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper routines */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* Compute the least run-sum of the prime indices of n. */
seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;

    /* Factorization buffers (enough for any 64‑bit number). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t min_run = 0;
    bool first = true;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Prime index: pi(p-1) + 1. */
        seq_t idx = (seq_t)compute_pi(p - 1) + 1;

        seq_t run_sum = idx * (seq_t)e;

        if (first || run_sum < min_run)
        {
            min_run = run_sum;
            first   = false;
        }
    }

    return min_run;
}
