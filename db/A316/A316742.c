/*
 * A316742  Stepping through the Mersenne sequence (A000225) one step back,
 *          two steps forward.
 * https://oeis.org/A316742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-2)+1 for n>1, with a(0)=1, a(1)=0 Closed form:
 *            a(4k) = 2*4^k-1, a(4k+1)=4^k-1, a(4k+2)=4^(k+1)-1,
 *            a(4k+3)=2*4^k-1
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Use the modulus 4 classification to compute the term directly. */
    switch (n & 3u) {
    case 0: {                     /* n = 4k */
        seq_t v = pow_uint(4, n / 4);
        return v * 2 - 1;
    }
    case 1: {                     /* n = 4k+1 */
        seq_t v = pow_uint(4, (n - 1) / 4);
        return v - 1;
    }
    case 2: {                     /* n = 4k+2 */
        seq_t v = pow_uint(4, (n + 2) / 4);
        return v - 1;
    }
    case 3: {                     /* n = 4k+3 */
        seq_t v = pow_uint(4, (n - 3) / 4);
        return v * 2 - 1;
    }
    }
    /* Unreachable */
    return 0;
}
