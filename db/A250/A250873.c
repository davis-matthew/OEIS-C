/*
 * A250873  Number of (n+1) X (4+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2340*4^n - 2160*3^n + 540*2^n + 24)/3
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Arbitrary‑precision build: rely on operator* for the seq_t type. */
    seq_t pow2_n = pow2(n);
    seq_t pow3_n = pow3(n);
    seq_t pow4_n = pow4(n);
    return (seq_t)780 * pow4_n - (seq_t)720 * pow3_n + (seq_t)180 * pow2_n + (seq_t)8;
#else
    /* Fixed‑width unsigned build: use 128‑bit intermediate to avoid overflow. */
    unsigned __int128 pow2_n = (unsigned __int128)pow2(n);
    unsigned __int128 pow3_n = (unsigned __int128)pow3(n);
    unsigned __int128 pow4_n = (unsigned __int128)pow4(n);
    unsigned __int128 term1 = (unsigned __int128)780 * pow4_n;
    unsigned __int128 term2 = (unsigned __int128)720 * pow3_n;
    unsigned __int128 term3 = (unsigned __int128)180 * pow2_n;
    unsigned __int128 result = term1 - term2 + term3 + 8;
    return (seq_t)result;
#endif
}
