/*
 * A294305  Sum of the tenth powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (i^10 + (n-i)^10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 70, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: For n < 3 the sum is empty, giving 0. */
#include <stdint.h>  /* for fixed-width types, if needed */

static seq_t pow10_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t sum = 0;
    idx_t h = (n - 1) / 2;
    for (idx_t i = 1; i <= h; ++i) {
        seq_t t1 = pow10_seq((seq_t)i, 10);
        seq_t t2 = pow10_seq((seq_t)(n - i), 10);
        sum += t1 + t2;
    }
    return sum;
}
