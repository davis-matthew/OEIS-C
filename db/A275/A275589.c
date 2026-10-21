/*
 * A275589  The almost-natural numbers with prime digits removed.
 * https://oeis.org/A275589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th digit of the infinite concatenation of natural
 *            numbers after removing the prime digits 2,3,5,7
 * Range:     u8 n <= 4492, u16 n <= 4492, u32 n <= 4492, u64 n <= 4492,
 *            u128 n <= 4492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t num = 1;; ++num) {
        /* highest power of 10 not exceeding num */
        idx_t pow = 1;
        while (pow <= num / 10) pow *= 10;

        for (idx_t p = pow; p > 0; p /= 10) {
            int d = num / p % 10;           /* current digit */
            if (d == 2 || d == 3 || d == 5 || d == 7)
                continue;                  /* skip prime digits */

            ++count;
            if (count == n)
                return (seq_t)d;
        }
    }
}
