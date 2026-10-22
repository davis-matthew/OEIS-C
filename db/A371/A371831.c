/*
 * A371831  a(n) = numerator(Sum_{k=1..n} k^2/k!).
 * https://oeis.org/A371831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(Sum_{k=1..n} k^2/k!)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for unsigned __int128 values.   */
static inline unsigned __int128 gcd128(unsigned __int128 a,
                                       unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* A is the integer A_n = n! * Sum_{k=1..n} k^2/k!  */
    unsigned __int128 A = 0;
    unsigned __int128 fact = 1;          /* fact = k!  */

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (unsigned __int128)k;    /* k!    */
        A = (unsigned __int128)k * A + (unsigned __int128)k * (unsigned __int128)k;
    }

    /* Reduce A/n! to its lowest fraction form */
    unsigned __int128 g = gcd128(A, fact);
    A /= g;

    return (seq_t)A;                    /* fits into seq_t for all n whose exact value fits */
}
