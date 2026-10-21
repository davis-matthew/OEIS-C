/*
 * A255825  A self-generating sequence: a(n) = n for n < 5; a(5n) = a(n); if
 *          every 5th term (a(5), a(10), a(15),...) is deleted, this gives
 *          back the original sequence.
 * https://oeis.org/A255825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n/5) if n % 5 == 0, otherwise a(n – floor(n/5)).
 *            Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce n until it is < 5, at which point the value is n itself. */
    while (n >= 5) {
        if (n % 5 == 0)
            n /= 5;
        else
            n -= n / 5;
    }
    return (seq_t)n;
}
