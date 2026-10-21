/*
 * A277593  Numbers k such that k/10^m == 6 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   numbers whose last non‑zero decimal digit is 6
 * Range:     u8 n <= 27, u16 n <= 7280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour below offset is irrelevant */

    idx_t count = 0;
    idx_t x = 0;

    while (count < n)
    {
        ++x;
        idx_t t = x;
        /* remove trailing zeros */
        while ((t % 10) == 0) t /= 10;
        if ((t % 10) == 6) ++count;
    }

    return (seq_t)x;
}
