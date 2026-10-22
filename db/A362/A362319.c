/*
 * A362319  a(n) = n! * Sum_{k=0..floor(n/5)} (n/5)^k / (k! * (n-5*k)!).
 * https://oeis.org/A362319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/5⌋} (n/5)^k / (k! * (n-5k)!)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = 0 term is always 1 */
    seq_t sum = (seq_t)1;
    seq_t term = (seq_t)1;
    idx_t maxk = n / 5;

    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t start = n - 5 * (k - 1);

        /* product of five consecutive decreasing integers starting at start */
        seq_t prod = (seq_t)1;
        prod *= (seq_t)(start);
        prod *= (seq_t)(start - 1);
        prod *= (seq_t)(start - 2);
        prod *= (seq_t)(start - 3);
        prod *= (seq_t)(start - 4);

        term = term * (seq_t)n;
        term = term * prod;
        term = term / ((seq_t)5 * (seq_t)k);

        sum += term;
    }

    return sum;
}
