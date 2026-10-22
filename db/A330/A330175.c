/*
 * A330175  a(n) = n + floor(ns/r) + floor(nt/r), where r = sqrt(5) - 2, s =
 *          sqrt(5) - 1, t = sqrt(5).
 * https://oeis.org/A330175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(ns/r) + floor(nt/r) where r = sqrt(5)-2, s =
 *            sqrt(5)-1, t = sqrt(5)
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‐root for unsigned __int128.
   Returns floor(√x).  */
static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 r   = 0;
    unsigned __int128 bit;

    /* Find the highest power of two ≤ x.  */
    bit = (unsigned __int128)1 << 127;
    while (bit > 0 && bit > x)  bit >>= 1;

    /* Binary search for sqrt.  */
    while (bit) {
        unsigned __int128 temp = r + bit;
        if (temp <= x / temp)   r = temp;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, harmless */

    seq_t N   = (seq_t)n;
    unsigned long long n64 = (unsigned long long)n;

    /* floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    unsigned __int128 m1   = (unsigned __int128)5 * n64 * n64;
    seq_t floor1           = (seq_t)isqrt128(m1);

    /* floor(2n*sqrt(5)) = floor(sqrt(20*n^2)) */
    unsigned __int128 m2   = (unsigned __int128)20 * n64 * n64;
    seq_t floor2           = (seq_t)isqrt128(m2);

    return N * (seq_t)9 + floor1 + floor2;
}
