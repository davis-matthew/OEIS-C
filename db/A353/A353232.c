/*
 * A353232  a(n) is the number of ways to split [n] = {1,2,...,n} into two
 *          (possibly empty) complementary intervals {1,2,...,i} and
 *          {i+1,i+2,...,n} and then, if both intervals are nonempty, select 2
 *          nonempty blocks/cells (i.e., subintervals) from each of them, or
 *          if one of the intervals is empty, select 2 no...
 * https://oeis.org/A353232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) + C(n+1,5)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a multiplicative computation for C(n+1,5) to avoid overflow. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, but guard for safety */

    /* First part: n*(n-1) */
    seq_t part1 = (seq_t)n * ((seq_t)n - 1);

    /* Second part: C(n+1,5) */
    seq_t part2 = 0;
    if (n >= 4) {
        idx_t m = n + 1;  /* compute C(m,5) */
        seq_t res = 1;
        for (idx_t k = 1; k <= 5; ++k) {
            res = res * (seq_t)(m - 5 + k);   /* multiply by the next numerator term */
            res = res / (seq_t)k;              /* exact division by the denominator term */
        }
        part2 = res;
    }

    return part1 + part2;
}
