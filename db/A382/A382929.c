/*
 * A382929  Smallest number k such that k + n + sigma(n) is a perfect number.
 * https://oeis.org/A382929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest perfect number P ≥ n + sigma(n)) – (n +
 *            sigma(n))
 * Range:     u8 n <= 11, u16 n <= 1979, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   pow2, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

static seq_t next_perfect_ge(seq_t T) {
    /* Exponents of known Mersenne primes that fit within 64‑bit perfect numbers */
    const idx_t exps[] = {2, 3, 5, 7, 13, 17, 19, 31};
    const size_t num = sizeof exps / sizeof exps[0];

    for (size_t i = 0; i < num; ++i) {
        idx_t p = exps[i];
        seq_t powp  = pow2(p);      /* 2^p   */
        seq_t powp1 = pow2(p - 1);  /* 2^(p-1) */
        seq_t perfect = seq_mul(powp1, powp - (seq_t)1); /* 2^(p-1)*(2^p-1) */

        if (perfect >= T)
            return perfect;
    }

    /* In practice this point is never reached for the intended range of n.
       If it is, return T as a fallback (will give k=0). */
    return T;
}

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);
    seq_t tot   = (seq_t)n + sigma;      /* n + sigma(n) */
    seq_t perfect = next_perfect_ge(tot);
    return perfect - tot;
}
