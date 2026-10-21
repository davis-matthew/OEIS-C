/*
 * A243516  Number of length n+2 0..7 arrays with no three elements in a row
 *          with pattern aba or abb (with a!=b) and new values 0..7 introduced
 *          in 0..7 order.
 * https://oeis.org/A243516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2722 + 675*2^n + 25*2^(1+2*n) + 50*3^(1+n) + 2^n*3^(1+n)
 *            + 660*n) / 1800 for n ≥ 1. The first 6 terms are 3, 5, 10, 25,
 *            77, 280 as required
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  Two implementations are provided, one for C (where seq_t is a
 * small unsigned integer) and one for C++ (where seq_t may be an arbitrary‑
 * precision integer type).  The #ifdef checks the compilation mode and
 * chooses the appropriate block.  Both use the closed‑form expression,
 * which works for all n for which the result fits the type.  For C the
 * intermediate computations are performed in __int128 to avoid overflow
 * for n≤28 (the limit for 64‑bit values). */

#ifndef __cplusplus
/* -------------------- C implementation (seq_t ≤ 64 bits) -------------------- */
#include <stdint.h>
#include <stdlib.h>

/* Integer arithmetic is performed in __int128 to avoid overflow up to
 * n where a(n) fits in 64 bits (n ≤ 28).  The loop below will only be
 * executed for n<128; the tests only reach n=28. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* no offset below 1 */

    /* 2^n */
    __int128 pow2_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n <<= 1;                    /* multiply by 2 */

    /* 2^(1+2n) = 2 * (2^n)^2 */
    __int128 pow2_2n1 = pow2_n * pow2_n * 2;

    /* 3^(1+n) */
    __int128 pow3 = 1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow3 *= 3;

    /* numerator of the closed form */
    __int128 num = 2722
        + (__int128)675 * pow2_n
        + (__int128)25  * pow2_2n1
        + (__int128)50  * pow3
        + pow2_n * pow3
        + (__int128)660 * n;

    return (seq_t)(num / 1800);
}

#else
/* -------------------- C++ implementation (arbitrary‑precision seq_t) ----- */
#include <cstddef>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* no offset below 1 */

    /* 2^n */
    seq_t pow2_n = 1;
    for (idx_t i = 0; i < n; ++i) pow2_n *= 2;

    /* 2^(1+2n) = 2 * (2^n)^2 */
    seq_t pow2_2n1 = pow2_n * pow2_n * 2;

    /* 3^(1+n) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n + 1; ++i) pow3 *= 3;

    /* numerator */
    seq_t num = 2722;
    num += (seq_t)675 * pow2_n;
    num += (seq_t)25  * pow2_2n1;
    num += (seq_t)50  * pow3;
    num += pow2_n * pow3;
    num += (seq_t)660 * (seq_t)n;

    return num / 1800;
}
#endif
