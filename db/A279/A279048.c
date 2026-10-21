/*
 * A279048  a(n) = 0 whenever n is a practical number (A005153) otherwise
 *          least powers of 2 that when multiplied by n becomes practical.
 * https://oeis.org/A279048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a practical number, otherwise the smallest
 *            non‑negative integer k such that n·2^k is practical
 * Range:     u8 n <= 1411, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Determine whether a 64‑bit integer is practical.
   The algorithm follows the characterization:
   For the prime factorisation
        n = Π p_i^e_i   (p_i in ascending order)
   let s_0 = 1 and for i>0
        s_i = s_{i-1} · σ(p_i^e_i)
   where σ is the sum‑of‑divisors function.
   Then n is practical iff for all i>0
        p_i ≤ s_{i-1} + 1.
*/
static bool is_practical_uint64(uint64_t n)
{
    if (n == 1) return true;          /* 1 is practical */

    uint64_t primes[64];
    uint64_t exps[ 64];
    size_t   cnt;
    factor_u64(n, primes, exps, &cnt); /* factorise n */

    /* The smallest prime must be 2. */
    if (primes[0] != 2) return false;

    seq_t cumulative = 1;              /* s_{i-1} in the description */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Condition p_i ≤ s_{i-1} + 1 */
        if ((seq_t)p > cumulative + 1) return false;

        /* σ(p^e) = 1 + p + p^2 + ... + p^e */
        seq_t sigma = 1;
        seq_t term  = 1;
        for (uint64_t j = 0; j < e; ++j) {
            term = term * p;
            sigma += term;
        }

        cumulative *= sigma;  /* update s_i */
    }

    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; for n=1.. */
    uint64_t nn = (uint64_t)n;

    if (is_practical_uint64(nn)) return 0;   /* n itself is practical */

    uint64_t m  = nn;
    int      k  = 0;

    /* Double until a practical number is reached.  The search
       terminates quickly for the ranges covered by the tests. */
    while (!is_practical_uint64(m)) {
        ++k;
        m <<= 1;                     /* m = m·2 */
    }

    return (seq_t)k;
}
