/*
 * A341677  Number of strictly inferior prime-power divisors of n.
 * https://oeis.org/A341677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strictly inferior prime‑power divisors of n
 * Range:     u8 n <= 3727, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A divisor d|n is strictly inferior if d < n/d.
   For a prime power d = p^k we have d < n/d ⇔ d^2 < n ⇔ p^(2k) < n.
   Thus we count, for each prime p that divides n, all exponents 1 ≤ k ≤ e
   such that p^(2k) < n.
*/

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt = 0;

    /* factorize N into primes[] and exps[] */
    factor_u64(N, primes, exps, &cnt);

    seq_t count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t d = (seq_t)p;
        for (uint64_t k = 1; k <= e; ++k) {
            /* stop if d^2 ≥ N */
            if (d * d >= (seq_t)N)
                break;
            count += (seq_t)1;
            d *= (seq_t)p;
        }
    }
    return count;
}
