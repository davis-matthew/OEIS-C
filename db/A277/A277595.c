/*
 * A277595  Numbers k such that k/10^m == 8 mod 10, where 10^m is the greatest
 *          power of 10 that divides k.
 * https://oeis.org/A277595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 7279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Numbers k such that k/10^m == 8 (mod 10), where 10^m is the greatest power
 * of 10 that divides k.  In other words, the last non‑zero digit of k is 8.
 */
seq_t a(idx_t n)
{
    idx_t     count = 0;    /* how many qualifying numbers have been found */
    seq_t     cur   = 1;    /* current candidate number */

    for (;; cur = cur + 1)  /* loop forever, increment cur each time */
    {
        seq_t tmp = cur;
        /* remove trailing zeros */
        while ((tmp % 10) == 0)
            tmp = tmp / 10;

        /* test last non‑zero digit */
        if ((tmp % 10) == 8)
        {
            count = count + 1;
            if (count == n)
                return cur;
        }
    }
}
