/*
 * A319653  Write n in 5-ary, sort digits into increasing order.
 * https://oeis.org/A319653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 311, u16 n <= 3124, u32 n <= 3124, u64 n <= 3124,
 *            u128 n <= 3124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by converting n to base 5, sorting its digits
   in increasing order, and interpreting the result again as a
   base‑5 number.  */
seq_t a(idx_t n)
{
    unsigned cnt[5] = {0};          /* counts of digits 0…4 */
    idx_t t = n;

    /* Count base‑5 digits of n */
    while (t > 0) {
        cnt[(int)(t % 5)]++;
        t /= 5;
    }

    /* Build the sorted number */
    seq_t res = 0;
    for (int d = 0; d <= 4; ++d) {
        for (unsigned k = 0; k < cnt[d]; ++k) {
            res = res * 5 + d;
        }
    }
    return res;
}
