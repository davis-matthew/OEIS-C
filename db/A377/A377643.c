/*
 * A377643  a(n) is the number of terms in the trajectory when the map x ->
 *          2+sopfr(x) is iterated, starting from x = n until x = 8, with
 *          sopfr = A001414.
 * https://oeis.org/A377643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of terms in the trajectory of the map
 * x -> 2 + sopfr(x) (sum of prime factors with multiplicity),
 * starting from x = n and stopping when x becomes 8.
 * This is the definition of OEIS sequence A377643. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* The following helper is supplied by the build: */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Count the starting term */
    seq_t count = 1;

    seq_t cur = (seq_t)n;

    /* Temporary buffers for prime factorisation of 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fac_cnt;

    while (cur != 8)
    {
        /* Factorise the current value (it will fit in 64 bits for the
           required range). */
        uint64_t cur64 = (uint64_t)cur;
        factor_u64(cur64, primes, exps, &fac_cnt);

        /* Compute sopfr: sum of prime factors with multiplicity. */
        seq_t sopfr = 0;
        for (size_t i = 0; i < fac_cnt; ++i)
            sopfr += (seq_t)primes[i] * (seq_t)exps[i];

        cur = 2 + sopfr;
        ++count;
    }

    return count;
}
