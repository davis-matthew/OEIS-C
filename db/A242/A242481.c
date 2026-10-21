/*
 * A242481  a(n) = ((n*(n+1)/2) mod n + sigma(n) mod n + antisigma(n) mod n) /
 *          n.
 * https://oeis.org/A242481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n*(n+1)/2) mod n + sigma(n) mod n + (n*(n+1)/2 -
 *            sigma(n)) mod n)/n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _sum_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_sum_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in the tests */

    /* triangular number */
    idx_t T = n * (n + 1) / 2;

    /* sum of divisors */
    idx_t sigma = _sum_divisors(n);

    /* antisigma = T - sigma (sum of non‑divisors < n) */
    idx_t antisigma = T - sigma;

    /* modular reductions */
    idx_t tmod = T % n;
    idx_t sigmod = sigma % n;
    idx_t antisigmod = antisigma % n;

    /* combine and divide by n (guaranteed to be an integer) */
    idx_t sum_mod = tmod + sigmod + antisigmod;
    return (seq_t)(sum_mod / n);
}
