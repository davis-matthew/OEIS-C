/*
 * A277570  Numbers k such that k/6^m == 4 (mod 6), where 6^m is the greatest
 *          power of 6 that divides k.
 * https://oeis.org/A277570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer k such that k/6^m == 4 (mod 6),
 *            where m is the greatest exponent with 6^m dividing k. (Computed
 *            by brute‑force scan.)
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = 1;
    idx_t count = 0;

    while (1) {
        seq_t tmp = k;
        /* Remove all factors of 6 to obtain the odd‑part not divisible by 3. */
        while (tmp % (seq_t)6 == 0) {
            tmp /= (seq_t)6;
        }
        if (tmp % (seq_t)6 == (seq_t)4) {
            ++count;
            if (count == n) {
                return k;
            }
        }
        ++k;
    }
}
