/*
 * A383766  a(n) is the number of numbers k (0 <= k < n) such that there exist
 *          solutions of x^3 + x == y^2 + 1 == k (mod n).
 * https://oeis.org/A383766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 367, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation: size of the intersection of the residue sets
 *  {x³ + x mod n | 0 ≤ x < n} and {y² + 1 mod n | 0 ≤ y < n}.  */

#include <stdint.h>
#include <stddef.h>   /* for size_t */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For the OEIS offset, n >= 1.  If n == 0, return 0. */
    if (n == 0)
        return (seq_t)0;

    const size_t sz = (size_t)n;
    uint8_t *hasX = (uint8_t *)malloc(sz);
    uint8_t *hasY = (uint8_t *)malloc(sz);
    if (!hasX || !hasY) {
        free(hasX);
        free(hasY);
        return (seq_t)0;   /* memory failure – unlikely in normal use */
    }
    for (size_t i = 0; i < sz; ++i) { hasX[i] = 0; hasY[i] = 0; }

    seq_t mod_n = (seq_t)n;

    /* Mark residues of y² + 1 modulo n. */
    for (idx_t y = 0; y < n; ++y) {
        seq_t y_seq = (seq_t)y;
        seq_t v = (y_seq * y_seq) % mod_n;      /* y²  mod n */
        v = (v + (seq_t)1) % mod_n;             /* y²+1 mod n */
        hasY[(size_t)v] = 1;
    }

    /* Mark residues of x³ + x modulo n. */
    for (idx_t x = 0; x < n; ++x) {
        seq_t x_seq = (seq_t)x;
        seq_t v = (x_seq * x_seq) % mod_n;      /* x²  mod n */
        v = (v * x_seq) % mod_n;                /* x³  mod n */
        v = (v + x_seq) % mod_n;                /* x³+x mod n */
        hasX[(size_t)v] = 1;
    }

    /* Count residues that appear in both sets. */
    seq_t count = 0;
    for (size_t i = 0; i < sz; ++i)
        if (hasX[i] && hasY[i])
            ++count;

    free(hasX);
    free(hasY);
    return count;
}
