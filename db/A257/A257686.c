/*
 * A257686  a(0) = 0, for n >= 1: a(n) = A099563(n) * A048764(n).
 * https://oeis.org/A257686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = d_m * m! where d_m is the most significant factorial base
 *            digit of n * (zeroing all other digits in the factorial
 *            representation). a(0)=0
 * Range:     u8 n <= 359, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* a(0) = 0 */

    idx_t fact = 1;                /* current factorial m! */
    idx_t m    = 1;                /* current factorial index (1! = 1) */

    /* Find largest m such that m! <= n.  This keeps fact = m! */
    while (fact * (m + 1) <= n) {
        m    += 1;
        fact  *= m;
    }

    idx_t d = n / fact;            /* most significant factorial digit */
    return (seq_t)(d * fact);      /* d * m!  (fits in seq_t when it is required to) */
}
