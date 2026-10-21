/*
 * A291304  The arithmetic function v_5(n,2).
 * https://oeis.org/A291304
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + gcd(n,10) - 2*gcd(n,5))/2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 2)
        return 0;
    seq_t n_val = (seq_t)n;
    seq_t g10 = gcd(n_val, (seq_t)10);
    seq_t g5  = gcd(n_val, (seq_t)5);
    seq_t t   = n_val + g10 - ((seq_t)2) * g5;
    return t / ((seq_t)2);
}
