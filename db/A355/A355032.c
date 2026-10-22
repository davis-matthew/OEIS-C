/*
 * A355032  a(n) is the maximum number of prime signatures of numbers with n
 *          divisors that have the same number of prime divisors (counted with
 *          multiplicity).
 * https://oeis.org/A355032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum number of distinct prime signatures of
 *            numbers with n divisors that have the same total number of prime
 *            divisors (counted with multiplicity). The sequence is computed
 *            by enumerating all unordered factorizations of n into factors
 *            ≥2; each factor corresponds to an exponent e = f...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n=1 the only number with one divisor is 1, which has an empty
   signature, so a(1)=1. */

#include <stdint.h>
#include <stdlib.h>

/* Recursive enumeration of factorizations of n_rem.
   `min_fac` enforces non‑decreasing order so each unordered factorization
   is generated exactly once.  `sum_exp` holds the sum of exponents
   so far.  `cnt` is an array with length n+1; cnt[Ω] counts signatures
   with total exponent Ω. */
static void recur(idx_t n_rem, idx_t min_fac, idx_t sum_exp, uint32_t *cnt)
{
    for (idx_t f = min_fac; f <= n_rem; ++f) {
        if (n_rem % f != 0)  /* not a divisor */
            continue;

        if (f == n_rem) {        /* final factor, finish signature */
            cnt[sum_exp + (f - 1)]++;
        } else {
            /* recurse with the remaining part, using current factor as new minimum */
            recur(n_rem / f, f, sum_exp + (f - 1), cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1)                     /* only the number 1 */
        return (seq_t)1;

    size_t sz = (size_t)(n + 1);    /* array size for Ω in [0..n] */
    uint32_t *cnt = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!cnt)                       /* allocation failure (should not happen) */
        return (seq_t)0;

    recur(n, 2, 0, cnt);            /* start enumeration with factor ≥2 */

    uint32_t best = 0;
    for (size_t i = 0; i < sz; ++i)
        if (cnt[i] > best)
            best = cnt[i];

    free(cnt);
    return (seq_t)best;
}
