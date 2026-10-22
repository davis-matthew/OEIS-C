/*
 * A395540  a(n) = floor(log_2(n)) + floor(log_2(2*n/3)) + 3.
 * https://oeis.org/A395540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_2(n)) + floor(log_2(2*n/3)) + 3
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(v)) for v > 0, return 0 if v == 0 (edge case handled elsewhere). */
static inline unsigned floor_log2_uint64(idx_t v)
{
    unsigned r = 0;
    while (v >>= 1) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; n==0 is not used */

    /* term1 = floor(log2(n)) */
    unsigned t1 = floor_log2_uint64(n);

    /* term2 = floor(log2(2*n/3)) */
    long long term2;
    if (n <= 1) {
        /* For n = 1, 2*n/3 = 2/3 < 1 so log2 < 0 → floor = -1 */
        term2 = -1;
    } else {
        idx_t t = (2 * n) / 3;   /* integer division floor */
        if (t == 0) {
            term2 = -1;
        } else {
            term2 = floor_log2_uint64(t);
        }
    }

    long long result = (long long)t1 + term2 + 3;
    return (seq_t)result;
}
