/*
 * A334224  Consider a graph as defined in A306302 formed from a row of n
 *          adjacent congruent squares with the diagonals of all possible
 *          rectangles; a(n) is the minimum edge length of the squares such
 *          that the vertices formed by all intersections have integer x and y
 *          coordinates.
 * https://oeis.org/A334224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003418(2n-1) = LCM(1,2,…,2n-1) for n>1; a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Uses prime factorization: lcm = Π p^⌊log_p(m)⌋ over all primes p≤m */
static seq_t lcm_upto(idx_t m)
{
    if (m < 2)
        return (seq_t)1;   /* LCM(1) = 1 */

    /* Sieve of Eratosthenes to list primes up to m */
    size_t sz = (size_t)m + 1;
    /* Use a simple byte array for composites */
    unsigned char *is_composite = (unsigned char *)malloc(sz);
    if (!is_composite)
        return (seq_t)0;   /* allocation failure – unlikely */
    memset(is_composite, 0, sz);

    for (idx_t i = 2; i * i <= m; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= m; j += i)
                is_composite[j] = 1;
        }
    }

    seq_t result = (seq_t)1;

    for (idx_t p = 2; p <= m; ++p) {
        if (!is_composite[p]) {
            seq_t power = (seq_t)p;
            /* Increase power until p^k > m, then use last power ≤ m */
            while (power * (seq_t)p <= (seq_t)m) {
                power *= (seq_t)p;
            }
            result *= power;
        }
    }

    free(is_composite);
    return result;
}

/* Compute LCM of all integers from 1 to m (inclusive) */

/* Sequence function required by the test harness */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;

    idx_t m = (idx_t)2 * n - (idx_t)1;
    return lcm_upto(m);
}
