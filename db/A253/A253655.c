/*
 * A253655  Number of monic irreducible polynomials of degree 6 over
 *          GF(prime(n)).
 * https://oeis.org/A253655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^6 - p^3 - p^2 + p)/6 where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 327,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑indexed) */
    seq_t p = (seq_t)get_prime(n);

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic when seq_t is 64 bits or smaller to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        __int128 p128 = (__int128)p;
        __int128 p2   = p128 * p128;          /* p^2 */
        __int128 p3   = p2 * p128;            /* p^3 */
        __int128 p6   = p3 * p3;              /* p^6 */
        __int128 num  = p6 - p3 - p2 + p128;  /* p^6 - p^3 - p^2 + p */
        __int128 res  = num / 6;              /* divide by 6 */
        return (seq_t)res;
    }
#endif

    /* Fallback: use seq_t arithmetic directly (works for arbitrary‑precision seq_t) */
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p6 = p3 * p3;
    seq_t val = (p6 - p3 - p2 + p) / 6;
    return val;
}
