/*
 * A390775  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *          binomial(k,2*(n-3*k)).
 * https://oeis.org/A390775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} 2^k * 3^(n-3k) * binomial(k,2(n-3k))
 *            Recurrence: a(n) = 4*a(n-3) - 4*a(n-6) + 12*a(n-7), with a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=2, a(4)=0, a(5)=0, a(6)=4
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 46, u64 n <= 90, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases up to n = 6 */
    static const seq_t base[7] = {
        (seq_t)1, (seq_t)0, (seq_t)0,
        (seq_t)2, (seq_t)0, (seq_t)0,
        (seq_t)4
    };
    if (n <= 6)
        return base[(idx_t)n];

    /* Circular buffer of size 8 that holds the most recent terms.
       The recurrence only needs values up to index i-7. */
    seq_t buf[8];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = base[i];

    /* Compute iteratively from i = 7 up to n */
    for (idx_t i = 7; i <= n; ++i) {
        /* Order operations so intermediate additions come first
           to avoid unsigned underflow when later subtracting. */
        seq_t term = buf[(i - 3) & 7] * 4;
        term += buf[(i - 7) & 7] * 12;
        term -= buf[(i - 6) & 7] * 4;
        buf[i & 7] = term;
    }

    return buf[n & 7];
}
