/*
 * A379015  a(n) is the reversed non-adjacent form (NAF) representation of n.
 * https://oeis.org/A379015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the reversed non‑adjacent form (NAF) representation of n
 * Range:     i8 n <= 86, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t x   = n;               /* implicit cast to seq_t */

    while (x != 0) {
        if (x & 1) {                                   /* odd */
            seq_t mod4 = x & 3;                        /* n mod 4 */
            int digit = (mod4 == 1) ? 1 : -1;
            ans = ans * 2 + digit;
            x = (x - digit) >> 1;
        } else {                                       /* even */
            ans = ans * 2;                            /* ans <<= 1;
                                                        but avoid UB of shifting negatives */
            x >>= 1;
        }
    }
    return ans;
}
