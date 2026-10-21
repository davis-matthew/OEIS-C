/*
 * A277549  Numbers k such that k/4^m == 1 (mod 4), where 4^m is the greatest
 *          power of 4 that divides k.
 * https://oeis.org/A277549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th positive integer k such that k/4^m ≡ 1 (mod 4),
   where 4^m is the greatest power of 4 dividing k. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        seq_t tmp = (seq_t)k;
        /* Remove the greatest power of 4 that divides k. */
        while (tmp % (seq_t)4 == (seq_t)0) {
            tmp /= (seq_t)4;
        }
        /* Check the defining congruence. */
        if (tmp % (seq_t)4 == (seq_t)1) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
