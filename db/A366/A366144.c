/*
 * A366144  a(n) = n/d(n) if d(n) | n, otherwise a(n) = n*d(n), where d(n) =
 *          A000005(n) is the number of divisors of n.
 * https://oeis.org/A366144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/d(n) if d(n) | n, else a(n) = n*d(n) where d(n) =
 *            number of divisors of n (OEIS A000005)
 * Range:     u8 n <= 41, u16 n <= 2099, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* n is assumed >= 1 */
    seq_t nn = (seq_t)n;
    idx_t d = divisor_count_u64((uint64_t)n);
    seq_t dd = (seq_t)d;

    if (nn % dd == 0)
        return nn / dd;
    else
        return nn * dd;
}
