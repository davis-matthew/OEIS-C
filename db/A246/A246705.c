/*
 * A246705  Position of first n in A246694 (read as sequence with offset
 *          changed to 1); complement of A246706.
 * https://oeis.org/A246705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 - floor((sqrt(4*n-3)-1)/2)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   sqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sqrt_u128.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n >= 1. For n == 0 we return 0. */
    if (n == 0) return (seq_t)0;

    /* Compute s = floor(sqrt(4*n-3)) */
    unsigned __int128 val = (unsigned __int128)4 * n - 3;
    idx_t s = sqrt_u128(val);

    /* t = floor((s-1)/2) = floor((sqrt(4*n-3)-1)/2) */
    idx_t t = (s - 1) / 2;

    /* a(n) = 2*n - 1 - t */
    unsigned __int128 res = (unsigned __int128)2 * n - 1 - t;
    return (seq_t)res;
}
