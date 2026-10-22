/*
 * A340251  a(n) is the index of the bit that was inverted in A340250(n) to
 *          get A340250(n+1).
 * https://oeis.org/A340251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   trailing_zeros_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A340251
 *
 *  OEIS definition: a(n) is the index of the bit that was inverted in
 *  A340250(n) to get A340250(n+1).  A340250(0)=0 and
 *  A340250(n+1)=A340250(n) XOR 2^a(n).
 *
 *  Closed‑form formula derived from the binary representation of n+1:
 *
 *      Let m = n + 1.
 *      L = floor_log2(m)               // position of the most significant bit
 *      T = number of trailing zeros in m
 *      if (m is a power of two)        // i.e. (m & (m-1)) == 0
 *          a(n) = L                     // L == log2(m)
 *      else
 *          a(n) = L - T - 1
 *
 *  The sequence starts 0,1,0,2,1,0,1,3,2,1,2,0,2,1,2,4,3,2,3,1,3,2,3,0,...
 *  Complexity is O(log n), well under the 2 s threshold for n≈10 000.
 */

#include <stdbool.h>
#include "../utils/trailing_zeros_u64.h"

static idx_t floor_log2_u64(idx_t v)
{
    /* v > 0 */
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Compute m = n + 1 in idx_t to avoid overflow. */
    idx_t m = n + 1;

    /* L = floor_log2(m)  */
    idx_t L = floor_log2_u64(m);

    /* Check if m is a power of two. */
    bool is_power_two = (m & (m - 1)) == 0;

    if (is_power_two) {
        /* a(n) = L for powers of two. */
        return (seq_t)L;
    } else {
        /* T = number of trailing zeros in m. */
        idx_t T = trailing_zeros_u64(m);
        idx_t result = L - T - 1;
        return (seq_t)result;
    }
}
