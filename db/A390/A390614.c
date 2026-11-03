/*
 * A390614  a(n) = Kronecker symbol (-35/n) = (n/35).
 * https://oeis.org/A390614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (-35/n) = (n/35)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Legendre symbol (a|p) for an odd prime p.
   Returns 1 or -1.  a may be larger than p; it is reduced modulo p. */
static inline int legendre_small(uint64_t a, uint64_t p)
{
    uint64_t base = a % p;                 /* reduce base modulo p */
    uint64_t exp  = (p - 1) >> 1;          /* (p-1)/2 */
    uint64_t res  = 1;

    while (exp) {
        if (exp & 1U)
            res = (uint64_t)((__int128)res * base % p);
        base = (uint64_t)((__int128)base * base % p);
        exp >>= 1U;
    }
    return (res == p - 1) ? -1 : (int)res; /* 1 or -1 */
}

seq_t a(idx_t n)
{
    /* a(0) = 0 by convention */
    if (n == 0)
        return (seq_t)0;

    /* If n shares a factor with 35, the Kronecker symbol is 0. */
    if ((n % 5ULL) == 0 || (n % 7ULL) == 0)
        return (seq_t)0;

    /* (n/35) = (n/5) * (n/7) for all n coprime with 35 */
    int res = 1;
    res *= legendre_small(n, 5ULL);
    res *= legendre_small(n, 7ULL);

    return (seq_t)res;
}
