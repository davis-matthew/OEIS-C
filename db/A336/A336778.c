/*
 * A336778  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the multiplication f <- f*n started at
 *          f=1 differs from the exact value n^a(n), when the multiplication
 *          is performed using 64-bit double precision floats according to the
 *          IEEE 754 standard.
 * https://oeis.org/A336778
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) – least number of repetitions so that the floating‑point
    product f <- f*n (starting from f=1.0) differs from the exact
    integer n^k.
    For n a power of two (n = 2^m) all intermediate results are exact
    powers of two until the exponent exceeds 1023 – in that case the
    result becomes +∞.  Therefore
        a(2^m) = floor(1023/m) + 1
    For all other n the first deviation occurs when the number of
    trailing zeros of n^k becomes smaller than the number of bits that
    must be eliminated to reduce n^k to a 53‑bit significand:
        let bitlen = ⌊k·log₂n⌋ + 1
        need    = max(0, bitlen − 53)   // trailing zeros required
        trail   = v₂(n) · k
        a(n)    = smallest k ≥ 1 with trail < need.
*/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n≥2 – values below are irrelevant */
    if (n < 2) {
        return 0;   /* arbitrary placeholder */
    }

    /* check whether n is a power of two */
    if ((n & (n - 1)) == 0) {
        /* n = 2^m, m = v₂(n) */
        uint64_t m    = 0;
        uint64_t tmp  = n;
        while ((tmp & 1ULL) == 0) {
            ++m;
            tmp >>= 1;
        }
        /* a(n) = floor(1023/m) + 1 */
        return (seq_t)((1023 / m) + 1);
    }

    /* n is not a power of two */
    /* v₂(n) – number of factors of 2 in n */
    uint64_t v2_n = 0;
    uint64_t tmp  = n;
    while ((tmp & 1ULL) == 0) {
        ++v2_n;
        tmp >>= 1;
    }

    /* pre‑compute log₂(n) as a double – safe to high precision */
    const double log2n = log2((double)n);

    /* iterate k from 1 upward */
    uint64_t k = 1;
    for (;; ++k) {
        /* bit length of n^k : floor(k·log₂n)+1 */
        unsigned bitlen = (unsigned)(floor(log2n * (double)k) + 1.0);

        unsigned need = (bitlen > 53) ? (bitlen - 53) : 0;

        unsigned trail = (unsigned)(v2_n * k);

        if (trail < need) {
            return (seq_t)k;
        }
    }
}
