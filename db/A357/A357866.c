/*
 * A357866  a(n) is the greatest remainder of n divided by its sum of digits
 *          in any base > 1.
 * https://oeis.org/A357866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the greatest remainder of n divided by the sum of the
 *            digits of n written in any base b > 1. Precisely, a(n) =
 *            max_{b>1} ( n mod S_b(n) ), where S_b(n) is the sum of digits of
 *            n in base b
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety, though sequence starts at index 1 */

    seq_t sn = (seq_t)n;
    seq_t maxrem = 0;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t base = (seq_t)b;
        seq_t temp = sn;
        seq_t sum = 0;
        while (temp != 0) {
            sum += temp % base;
            temp /= base;
        }
        /* sum is always ≥ 1 for n ≥ 1; safe to compute modulo */
        seq_t rem = sn % sum;
        if (rem > maxrem) maxrem = rem;
    }
    return maxrem;
}
