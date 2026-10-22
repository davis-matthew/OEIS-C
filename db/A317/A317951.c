/*
 * A317951  An S-automatic sequence for the system S = (a*b*, {a,b}, a<b).
 * https://oeis.org/A317951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010873(k) if n is the k-th triangular number (n =
 *            k*(k+1)/2), otherwise a(n) = 4. Here A010873(k) = k mod 4
 * Range:     u8 n <= 9511, u16 n <= 9511, u32 n <= 9511, u64 n <= 9511,
 *            u128 n <= 9511, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute integer square root of a 128‑bit unsigned integer (floor). */
static unsigned long long isqrt128(__uint128_t x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 63;      /* highest power of two <= sqrt(2^128-1) */
    while (bit != 0) {
        unsigned long long next = r | bit;
        __uint128_t test = (__uint128_t)next * next;
        if (test <= x)
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Return the sequence value a(n). */
seq_t a(idx_t n)
{
    /* m = 1 + 8*n  (use 128‑bit to avoid overflow). */
    __uint128_t m = (__uint128_t)8 * n + 1;

    /* s = floor(sqrt(m)). */
    unsigned long long s = isqrt128(m);

    /* Check if m is a perfect square. */
    if ((__uint128_t)s * s != m)
        return (seq_t)4;

    /* k = (s - 1) / 2. */
    unsigned long long k = (s - 1) / 2;

    /* Return k mod 4.  The result fits into any plain integer type. */
    return (seq_t)(k & 3);
}
