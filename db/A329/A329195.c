/*
 * A329195  a(n) = floor(log_5(n^2)) = floor(2 log_5(n)).
 * https://oeis.org/A329195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_5(n^2))
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* computing n^2 in the same type as the sequence value
       (the sequence type may be uint64_t or arbitrary-precision,
       but this product fits for the intended input range */
    seq_t n_sqr = (seq_t)n * (seq_t)n;

    seq_t power = 1;
    idx_t k = 0;
    while (power <= n_sqr) {
        k++;

        /* If the next multiplication would overflow n_sqr,
           we can stop early because the loop will exit afterwards.
           The division is safe because n_sqr is non–negative. */
        if (power > n_sqr / 5)
            break;

        power *= 5;
    }

    return (seq_t)(k - 1);  /* the last k that satisfied power <= n^2 */
}
