/*
 * A348538  Number of partitions of n into 3 parts whose smallest part divides
 *          n.
 * https://oeis.org/A348538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j|n, j <= n/3} ( floor((n - j)/2) - j + 1 )
 * Range:     u8 n <= 83, u16 n <= 3359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. number of partitions of n into 3 parts where the smallest part divides n. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxj   = n / 3;              /* smallest part can be at most n/3 */
    for (idx_t j = 1; j <= maxj; ++j) {
        if (n % j != 0)               /* only divisors of n */
            continue;
        idx_t max_i = (n - j) / 2;    /* i <= (n - j)/2 guarantees i <= (n - i - j) */
        if (max_i < j)
            continue;                /* no valid i for this j */
        total += (seq_t)(max_i - j + 1);   /* count of i values for this j */
    }
    return total;
}
