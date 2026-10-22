/*
 * A373461  a(n) = s - t where s = ceiling(sqrt(n*i)), t = sqrt(m), and m =
 *          s^2 mod n, for the smallest positive integer i for which m is
 *          square.
 * https://oeis.org/A373461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373461:  a(n) = s - t
 * where
 *   s = ceil(sqrt(n * i))
 *   m = s^2 mod n
 *   t = sqrt(m)
 *   i is the smallest positive integer for which m is a perfect square
 * The function iterates i from 1 upward until m becomes a square.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit value.
 * Returns floor(sqrt(x)) using a bit‑by‑bit algorithm. */
static inline unsigned long long isqrt_ull(unsigned long long x)
{
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 62;   /* highest power of 4 <= 2^64-1 */

    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  The index 0 case is
     * irrelevant for the OEIS offset but is harmless here. */
    if (n == 0) return (seq_t)0;

    unsigned long long N = (unsigned long long)n;

    for (unsigned long long i = 1; ; ++i) {
        unsigned long long prod = N * i;           /* n * i */
        unsigned long long f = isqrt_ull(prod);    /* floor(sqrt(prod)) */
        unsigned long long s = f;
        if (f * f != prod) s = f + 1;              /* ceil(sqrt(prod)) */

        unsigned long long s_sq = s * s;
        unsigned long long m = s_sq % N;           /* s^2 mod n */

        unsigned long long t = isqrt_ull(m);       /* floor(sqrt(m)) */
        if (t * t == m) {                          /* m is a perfect square */
            unsigned long long result = s - t;
            return (seq_t)result;
        }
    }
}
