/*
 * A342719  Array read by ascending antidiagonals: T(k, n) is the sum of the
 *          consecutive positive integers from 1 to (n - 1)*k placed along the
 *          perimeter of an n-th order perimeter-magic k-gon.
 * https://oeis.org/A342719
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k,n) read by ascending antidiagonals. Here T(k,n) =
 *            ((n-1)*k * ((n-1)*k + 1)) / 2, with offset 3
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_isqrt_u64_from_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_isqrt_u64_from_uint128.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                   /* offset; behaviour below 3 is irrelevant */

    idx_t i = n - 3;                       /* 0‑based position in the antidiagonal enumeration */

    /* Find M = s - 5, where s = k + n ;   M satisfies M(M+1)/2 > i >= (M-1)M/2  */
    __int128 val = (__int128)8 * i + 1;
    idx_t t = floor_isqrt_u64_from_uint128(val);   /* t = floor(sqrt(1 + 8*i))               */
    idx_t M = (t + 1) / 2;                          /* M = (t+1)/2  */

    idx_t prev = (( __int128)(M - 1) * M) / 2;      /* terms before current sum block    */
    idx_t pos = i - prev;                           /* 0‑based index within block       */

    idx_t k   = M + 2 - pos;        /* k decreases from (M+2) down to 3          */
    idx_t n_val = pos + 3;          /* corresponding n                           */

    seq_t u = (seq_t)k * (seq_t)(n_val - 1);        /* u = (n-1)*k                           */
    seq_t res = u * (u + 1) / 2;   /* triangular number of u                 */
    return res;
}
