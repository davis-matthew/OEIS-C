/*
 * A363861  Sequence related to chains in type D noncrossing partitions.
 * https://oeis.org/A363861
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)^(n/2-1) if n even a(n) = (n-1)^((n+1)/2) if n
 *            odd Offset: 3 (The sequence is zero for n < 3)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* If called with n < 3, return 0 (undefined behavior otherwise). */
    if (n < 3) return 0;

    seq_t base = (seq_t)(n - 1);

    if ((n & 1) == 0) {          /* even n */
        seq_t exp = (seq_t)(n / 2 - 1);
        seq_t pow = big_pow(base, exp);
        return pow * ((seq_t)(n - 2));
    } else {                     /* odd n */
        seq_t exp = (seq_t)((n + 1) / 2);
        return big_pow(base, exp);
    }
}
