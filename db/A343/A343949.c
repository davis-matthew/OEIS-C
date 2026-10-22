/*
 * A343949  Shortest distance from curve start to end along the segments of
 *          dragon curve expansion level n, and which is the diameter of the
 *          curve as a graph.
 * https://oeis.org/A343949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(2*n) = 3*2^n + 2*n - 4 (n >= 1) a(2*n+1) = 4*2^n +
 *            2*n - 2 (n >= 0)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: power of two using exponentiation by squaring.
   seq_mul is available from the utilities. */
#include "../utils/seq_mul.h"

static inline seq_t pow2_seq(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp != 0) {
        if (exp & 1) {
            result = seq_mul(result, base);
        }
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Even indices: n = 2*k, k >= 1 */
    if ((n & 1) == 0) {
        idx_t k   = n >> 1;               /* k = n/2 */
        seq_t pow = pow2_seq(k);          /* 2^k */
        return (seq_t)3 * pow + (seq_t)2 * (seq_t)k - (seq_t)4;
    }

    /* Odd indices: n = 2*k + 1, k >= 0 */
    idx_t k   = n >> 1;                   /* k = (n-1)/2 */
    seq_t pow = pow2_seq(k);              /* 2^k */
    return (seq_t)4 * pow + (seq_t)2 * (seq_t)k - (seq_t)2;
}
