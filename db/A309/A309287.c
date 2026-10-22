/*
 * A309287  Square array T(v, m), read by antidiagonals, for the Rogel-Klee
 *          arithmetic function: number of positive integers h in the set [m]
 *          for which gcd(h, m) is v-th-power-free, i.e., gcd(h, m) is not
 *          divisible by any v-th power of an integer > 1 (with v, m >= 1).
 * https://oeis.org/A309287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(v, m) where (v, m) is the element of the * square array
 *            read by antidiagonals. For a given n (1‑based) * find the
 *            smallest d such that d*(d+1)/2 ≥ n, then * pos = n – d*(d-1)/2,
 *            * v = pos, * m = d+1 – pos. * * T(v, m) – Rogel‑Klee arithmetic
 *            function: * T(v, m) = m * Π_{p prime, p^v | m}...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer exponentiation: base^exp. */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Compute T(v, m) for v ≥ 1, m ≥ 1. */
static seq_t compute_T(idx_t v, idx_t m)
{
    if (m == 1) return (seq_t)1;

    seq_t res = (seq_t)1;
    idx_t n   = m;

    /* Handle factor 2 separately. */
    if ((n & 1ULL) == 0) {
        idx_t e = 0;
        while ((n & 1ULL) == 0) { n >>= 1; ++e; }
        seq_t term = (e < v) ? pow_uint((seq_t)2, e)
                             : pow_uint((seq_t)2, e) - pow_uint((seq_t)2, e - v);
        res *= term;
    }

    /* Odd prime factors. */
    for (idx_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            seq_t term = (e < v) ? pow_uint((seq_t)p, e)
                                 : pow_uint((seq_t)p, e) - pow_uint((seq_t)p, e - v);
            res *= term;
        }
    }

    /* Remaining prime factor, if any. */
    if (n > 1) {
        idx_t e  = 1;
        seq_t term = (e < v) ? pow_uint((seq_t)n, e)
                             : pow_uint((seq_t)n, e) - pow_uint((seq_t)n, e - v);
        res *= term;
    }

    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used; n ≥ 1 in tests */

    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    idx_t prev  = d * (d - 1) / 2;
    idx_t pos   = n - prev;           /* 1‑based position on diagonal d */
    idx_t v     = pos;                /* row index */
    idx_t m     = d + 1 - pos;        /* column index */

    return compute_T(v, m);
}
