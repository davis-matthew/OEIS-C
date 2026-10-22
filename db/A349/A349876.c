/*
 * A349876  If n is divisible by 3, a(n) = n; otherwise n = 3k + r with r in
 *          {1, 2} and a(n) = a(5k + r + 3). a(n) = -1 if no multiple of three
 *          will be ever reached by iterating A353314.
 * https://oeis.org/A349876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n if n % 3 == 0 * a(n) = a(5*⌊n/3⌋ + (n%3) + 3) otherwise
 *            * Return -1 (represented as the maximum value of seq_t) if no
 *            multiple of 3 is ever reached
 * Range:     u8 n <= 1, u16 n <= 361, u32 n <= 9848, u64 n <= 9848,
 *            u128 n <= 9848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    /* safeguard against pathological cases – a very large number of steps indicates a loop to -1 */
    idx_t max_steps = 100000;   /* ample for all reasonable indices */
    while (m % 3 != 0) {
        if (--max_steps == 0)
            return (seq_t)-1;   /* sentinel for “no multiple of 3 reached” */
        seq_t k = m / 3;
        seq_t r = m % 3;        /* r is 1 or 2 */
        m = 5 * k + r + 3;
    }
    return m;
}
