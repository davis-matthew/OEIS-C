/*
 * A258016  Unlucky numbers removed at the stage three of Lucky sieve.
 * https://oeis.org/A258016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42 * ((n-1)/2) + (n%2 ? 19 : 39)
 * Range:     u8 n <= 12, u16 n <= 3120, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* zero is outside the sequence domain; return 0 for safety */
    if (n == 0)
        return 0;

    /* floor((n-1)/2) computed in idx_t, then cast to seq_t */
    seq_t k = (seq_t)((n - 1) / 2);
    seq_t base = k * (seq_t)42;

    if (n & 1)                 /* n is odd */
        return base + (seq_t)19;
    else                        /* n is even */
        return base + (seq_t)39;
}
