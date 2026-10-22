/*
 * A325403  Number of permutations of the multiset of prime factors of 2n
 *          whose first part is not 2.
 * https://oeis.org/A325403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008480(2n) - A008480(n)
 * Range:     u8 n <= 1763, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute C(n,k) for small n, k using only seq_t arithmetic. */
static seq_t binomial_mul(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Factorise n. */
    uint64_t primes[64], exps[64];   /* enough for any 64‑bit n */
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    idx_t tot = 0;          /* total number of prime factors (with multiplicity) */
    idx_t exp2 = 0;         /* exponent of 2 in n */

    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = exps[i];
        tot += e;
        if (primes[i] == 2ULL) exp2 = e;
    }

    /* A008480(n) = total! / product(exponent!); compute via multinomial. */
    seq_t A_n = 1;
    idx_t rem = tot;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = exps[i];
        A_n *= binomial_mul(rem, e);
        rem -= e;
    }

    /* a(n) = A_n * (tot - exp2) / (exp2 + 1). */
    if (tot == exp2) return 0;          /* difference is zero */
    idx_t diff = tot - exp2;
    idx_t denom = exp2 + 1;

    seq_t ans = A_n;
    ans *= (seq_t)diff;
    ans /= (seq_t)denom;
    return ans;
}
