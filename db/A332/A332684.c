/*
 * A332684  a(n) = Sum_{k=1..n} mu(gcd(n, k)) * floor(n/k).
 * https://oeis.org/A332684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(gcd(n, k)) * floor(n/k)
 * Range:     u8 n <= 60, u16 n <= 7252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependence on the signed MU function defined in utils/mu_of.h */

#include "../utils/mu_of.h"

/* local gcd for idx_t values */
static idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    long long sum = 0;                     /* signed accumulator */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t g = gcd_uint64(n, k);        /* gcd as idx_t */
        int mu = mu_of(g);                 /* signed Möbius value */
        idx_t f = n / k;                   /* floor(n/k) as idx_t */
        sum += mu * (long long)f;          /* accumulate signed */
    }
    return (seq_t)sum;                     /* cast back to seq_t */
}
