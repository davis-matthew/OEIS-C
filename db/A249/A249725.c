/*
 * A249725  Inverse permutation to A135764.
 * https://oeis.org/A249725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (((A003602(n)+A007814(n))^2 + A007814(n) -
 *            A003602(n))/2)
 * Range:     u8 n <= 46, u16 n <= 724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003602(n) = ceil(odd_part(n)/2) where odd_part(n) = n >> v2(n)   */
/* A007814(n) = v2(n) (number of trailing zeros in the binary expansion of n)   */

#include <stdint.h>

/* count trailing zeros of n (v2(n)) */
static inline idx_t _v2(idx_t n)
{
    idx_t c = 0;
    while ((n & 1) == 0)
    {
        n >>= 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* trailing zeros of n (A007814) */
    seq_t b = (seq_t)_v2(n);

    /* odd part of n = n >> b */
    seq_t odd = N >> (idx_t)b;      /* shift uses n's bit width */
    /* A003602 = ceil(odd_part/2) = (odd + 1)/2   */
    seq_t c = (odd + 1) / 2;

    seq_t s = c + b;

    /* compute the expression with a wider type to avoid overflow */
    unsigned long long t = (unsigned long long)s * (unsigned long long)s
                          + (unsigned long long)b
                          - (unsigned long long)c;

    seq_t res = 1 + (seq_t)(t / 2);
    return res;
}
