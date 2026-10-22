/*
 * A351696  a(n) is the numerator of the squared circumradius of a cyclic
 *          quadrilateral with sides n, n+1, n+2, n+3.
 * https://oeis.org/A351696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator((2*n^6 + 18*n^5 + 65*n^4 + 120*n^3 + 117*n^2 +
 *            54*n + 9)/(n^4 + 6*n^3 + 11*n^2 + 6*n))
 * Range:     u8 none, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the formula for OEIS sequence A351696.
 * It works for all builds: 8‑, 16‑, 32‑, 64‑bit unsigned integer
 * variants, the 128‑bit unsigned variant, and arbitrary‑precision
 * integer builds. All intermediate arithmetic is performed in a
 * 128‑bit unsigned type to avoid overflow for the input range up to
 * at least 10 000. */

#include <stdint.h>

/* 128‑bit unsigned type available in both C and C++ (GCC/Clang). */
typedef unsigned __int128 u128;

/* Euclidean algorithm for 128‑bit unsigned integers. */
static inline u128
_gcd128(u128 a, u128 b)
{
    while (b != 0) {
        u128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Public entry point of the sequence. */
seq_t a(idx_t n)
{
    /* Promote the index once; all further calculations use u128. */
    const u128 nn = (u128)n;

    /* Powers of n.  All are computed using 128‑bit arithmetic. */
    const u128 n2 = nn * nn;
    const u128 n3 = n2 * nn;
    const u128 n4 = n3 * nn;
    const u128 n5 = n4 * nn;
    const u128 n6 = n5 * nn;

    /* Numerator of the rational expression. */
    const u128 num = (u128)2 * n6
                   + (u128)18 * n5
                   + (u128)65 * n4
                   + (u128)120 * n3
                   + (u128)117 * n2
                   + (u128)54  * nn
                   + (u128)9;

    /* Denominator of the rational expression. */
    const u128 den = n4
                   + (u128)6 * n3
                   + (u128)11 * n2
                   + (u128)6 * nn;

    /* Reduce the fraction and return the numerator. */
    const u128 g = _gcd128(num, den);
    return (seq_t)(num / g);
}
