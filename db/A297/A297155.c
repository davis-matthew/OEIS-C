/*
 * A297155  a(1) = a(2) = 0, after which, a(n) = 1+a(n/2) if n is of the form
 *          4k+2, otherwise a(n) = a(A252463(n)).
 * https://oeis.org/A297155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(n) - 1 for n > 1; a(1) = a(2) = 0
 * Range:     u8 n <= 9973, u16 n <= 9973, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    idx_t m = n;
    seq_t cnt = 0;

    /* factor out 2 */
    if ((m & 1) == 0) {
        cnt++;
        while ((m & 1) == 0) m >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            cnt++;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1) cnt++;  /* remaining prime */

    return (seq_t)(cnt - 1);
}
