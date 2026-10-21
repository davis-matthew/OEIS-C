/*
 * A277569  Numbers k such that k/6^m == 3 (mod 6), where 6^m is the greatest
 *          power of 6 that divides k.
 * https://oeis.org/A277569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k such that k/6^m ≡ 3 (mod 6),
 *            where 6^m is the greatest power of 6 dividing k
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val = 0;
    while (1)
    {
        val++;                               /* start from 1 */
        if ((val % 3) != 0) continue;        /* k must be divisible by 3 */
        seq_t r = val;
        while (r % 6 == 0) r /= 6;           /* strip the greatest 6‑power factor */
        if (r % 6 == 3)                      /* rightmost nonzero base‑6 digit is 3 */
        {
            count++;
            if (count == n) return val;
        }
    }
}
