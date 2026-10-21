/*
 * A266443  Decimal representation of the n-th iteration of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=10, a(3)=67, for even n>=4:
 *            a(n)=29*2^(n/2), for odd n>3: a(n)=2^(2*n+1)-47*2^((n+1)/2)-1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)10;
    if (n == 3u) return (seq_t)67;

    /* Even indices (n >= 4) */
    if ((n & 1u) == 0u) {
        seq_t p = pow2(n / 2u);          /* 2^(n/2) */
        return (seq_t)29 * p;            /* 29 * 2^(n/2) */
    }

    /* Odd indices (n > 3) */
    seq_t p1 = pow2(2u * n + 1u);        /* 2^(2*n+1) */
    seq_t p2 = pow2((n + 1u) / 2u);      /* 2^((n+1)/2) */
    seq_t mul = (seq_t)47 * p2;          /* 47 * 2^((n+1)/2) */
    return p1 - mul - (seq_t)1;          /* (2^(2*n+1)) - (47*2^((n+1)/2)) - 1 */
}
