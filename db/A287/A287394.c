/*
 * A287394  Domination number for camel's graph on a 2 X n board.
 * https://oeis.org/A287394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ( floor((n+6)/9) + floor((n+7)/9) + floor((n+8)/9) )
 * Range:     u8 n <= 295, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow on addition */
    seq_t n_se = (seq_t)n;

    /* Compute each floor division term */
    seq_t t1 = (n_se + 6) / 9;
    seq_t t2 = (n_se + 7) / 9;
    seq_t t3 = (n_se + 8) / 9;

    /* Sum and multiply by 2 to obtain the result */
    return 2 * (t1 + t2 + t3);
}
