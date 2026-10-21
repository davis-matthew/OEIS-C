/*
 * A285950  Positions of 0's in A285949; complement of A285951.
 * https://oeis.org/A285950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(2n)   = 3*n - (popcount(n-1) % 2)
 *          a(2n+1) = 3*n + 1
 *
 * Here popcount(x) is the number of 1‑bits in x and %2 is the parity
 * (0 for even number of bits, 1 for odd).  The sequence starts with
 * a(1)=1, a(2)=3, a(3)=4, a(4)=5, … as in OEIS A285950.
 */

#include <stdint.h>

/* Compute the parity (popcount modulo 2) of a 64‑bit integer. */
static inline unsigned char parity64(uint64_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned char)(x & 1U);
}

seq_t a(idx_t n)
{
    if (n & 1U) {                     /* odd index: n = 2k+1   */
        idx_t k = n / 2U;             /* k = (n-1)/2           */
        return (seq_t)(3 * k + 1);
    } else {                          /* even index: n = 2k    */
        idx_t k = n / 2U;             /* k = n/2                */
        uint64_t km1 = (uint64_t)(k - 1U);
        unsigned char p = parity64(km1);
        return (seq_t)(3 * k - p);
    }
}
