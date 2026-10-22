/*
 * A345482  Numbers that are the sum of seven squares in five or more ways.
 * https://oeis.org/A345482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)‑a(11) are given explicitly; for n ≥ 12, a(n) = n + 57.
 *            (Recurrence a(n)=2·a(n−1)−a(n−2) holds for n>13.)
 * Range:     u8 n <= 198, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 11 initial terms of the sequence. */
    static const seq_t init[11] = {
        (seq_t)45, (seq_t)54, (seq_t)55, (seq_t)57, (seq_t)58,
        (seq_t)60, (seq_t)61, (seq_t)63, (seq_t)64, (seq_t)66,
        (seq_t)67
    };

    if (n <= 11) {
        /* n is 1‑based; table index is n‑1.  */
        return init[n - 1];
    }
    /* For all larger indices the sequence is simply n + 57. */
    return (seq_t)n + (seq_t)57;
}
