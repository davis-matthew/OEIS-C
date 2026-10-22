/*
 * A355402  Maximal GCD of seven positive integers with sum n.
 * https://oeis.org/A355402
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{ d | d divides n and d <= n/7 } (for n >= 7)
 * Range:     u8 n <= 319, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7) return 0;        /* offset is 7; values < 7 are undefined */
    seq_t best = 1;
    seq_t limit = (seq_t)n / 7; /* floor(n/7) */

    /* iterate over all divisors by checking up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t d_val = (seq_t)d;
            seq_t e_val = (seq_t)(n / d);

            if (d_val <= limit && d_val > best) best = d_val;
            if (e_val <= limit && e_val > best) best = e_val;
        }
    }
    return best;
}
