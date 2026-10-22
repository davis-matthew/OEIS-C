/*
 * A385662  Number of divisors d of n such that d^d == (-d)^d (mod n).
 * https://oeis.org/A385662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   d_power_divisible_by, divisor_count_u64, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385662: a(n) = number of divisors d of n such that d^d == (-d)^d (mod n). */
/* For n ≡ 0 (mod 4) : a(n) = τ(n/2) (divisor count).                 */
/* For n odd           : a(n) = |{ d | n : n | d^d }|.             */
/* For n = 2 m (m odd) : a(n) = a(m) + τ(m). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/d_power_divisible_by.h"

/* Recursive computation without caching (every call recomputes). */
static seq_t
compute_a(idx_t n)
{
    if ((n % 4) == 0) {                     /* n divisible by 4 */
        return (seq_t)divisor_count_u64(n / 2);
    }

    if ((n % 2) == 1) {                     /* n odd */
        if (n == 1)                         /* a(1) = 1 */
            return (seq_t)1;

        /* Factor n.  Use a small static buffer: n ≤ 10 000 → < 16 primes. */
        uint64_t primes[16], exps[16];
        size_t pcnt = 0;
        factor_u64(n, primes, exps, &pcnt);

        /* Enumerate all divisors of n.  The number of divisors ≤ 128. */
        uint64_t divs[256];
        size_t dcnt = 1;
        divs[0] = 1;

        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            uint64_t step = 1;
            size_t cur = dcnt;
            for (uint64_t e = 1; e <= exps[i]; ++e) {
                step *= p;
                for (size_t j = 0; j < cur; ++j)
                    divs[dcnt++] = divs[j] * step;
            }
        }

        /* Count divisors d with n | d^d */
        seq_t count = 0;
        for (size_t i = 0; i < dcnt; ++i) {
            uint64_t d = divs[i];
            if (d_power_divisible_by(d, n))
                count++;
        }
        return count;
    }

    /* n = 2 m with m odd */
    idx_t m = n / 2;
    return compute_a(m) + (seq_t)divisor_count_u64(m);
}

/* Public entry point */
seq_t a(idx_t n)
{
    return compute_a(n);
}
