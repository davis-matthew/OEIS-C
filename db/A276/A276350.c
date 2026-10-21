/*
 * A276350  Sum of digits of 1/n in factorial base.
 * https://oeis.org/A276350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of digits of the factorial‑base representation of 1/n */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t n_s = (seq_t)n;
    idx_t r = 1;   /* remainder of the division algorithm */
    idx_t k = 1;   /* factorial place counter */

    while (1) {
        seq_t prod = (seq_t)r * (seq_t)k; /* r * k may overflow idx_t */
        sum += prod / n_s;                /* digit in position k   */
        r = (idx_t)(prod % n_s);          /* new remainder         */
        if (r == 0)
            break;                       /* all digits found      */
        ++k;
    }
    return sum;
}
