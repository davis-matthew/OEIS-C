/*
 * A277567  Numbers k such that k/6^m == 1 (mod 6), where 6^m is the greatest
 *          power of 6 that divides k.
 * https://oeis.org/A277567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation: iterate over k and test k/6^m ≡ 1 (mod 6).  The
   condition is equivalent to “after removing all factors of 6,
   the remaining part ≡ 1 (mod 6)”.  The first 24 terms are
   1, 6, 7, 13, …, 114. */
seq_t a(idx_t n)
{
    if (n == 0)   /* behaviour below offset is undefined */
        return 0;

    idx_t   count = 0;
    seq_t   k     = 0;

    for (;;)
    {
        k++;                 /* next integer candidate */

        seq_t r = k;
        while (r % 6 == 0)
            r /= 6;          /* strip all powers of 6 */

        if (r % 6 == 1)      /* property holds */
        {
            count++;
            if (count == n)
                return k;
        }
    }
}
