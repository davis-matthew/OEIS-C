/*
 * A306536  The smallest integer k such that floor((2*n)^k/k) is an odd
 *          number.
 * https://oeis.org/A306536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer k such that floor((2*n)^k / k) is
 *            odd
 * Range:     u8 n <= 305, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* For n=0 the value is undefined; returning 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }
    for (idx_t k = 1; ; ++k) {
        seq_t mod = ((seq_t)k) << 1;           /* mod = 2*k   */
        seq_t base = ((seq_t)2 * (seq_t)n) % mod;
        seq_t rem  = mod_pow(base, k, mod);    /* (2*n)^k mod (2*k) */
        if (rem >= (seq_t)k) {
            return (seq_t)k;                   /* floor((2*n)^k / k) is odd */
        }
    }
}
