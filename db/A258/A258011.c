/*
 * A258011  Numbers remaining after the third stage of Lucky sieve.
 * https://oeis.org/A258011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-12) + 42 Initial values: a(1)=1, a(2)=3, a(3)=7,
 *            a(4)=9, a(5)=13, a(6)=15, a(7)=21, a(8)=25, a(9)=27, a(10)=31,
 *            a(11)=33, a(12)=37
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Residues modulo 42 that belong to the sequence (12 entries). */
    static const seq_t base[12] = {
        (seq_t)1, (seq_t)3, (seq_t)7, (seq_t)9,
        (seq_t)13, (seq_t)15, (seq_t)21, (seq_t)25,
        (seq_t)27, (seq_t)31, (seq_t)33, (seq_t)37
    };

    /* For the first 12 terms, return the pre‑computed values. */
    if (n <= 12) {
        return base[n - 1];
    }

    /* For n > 12 use the linear growth: each 12th step adds 42. */
    idx_t period = 12;
    idx_t period_index = (n - 1) % period;      /* 0 … 11 */
    idx_t periods     = (n - 1) / period;       /* number of full periods */

    return base[period_index] + (seq_t)periods * (seq_t)42;
}
