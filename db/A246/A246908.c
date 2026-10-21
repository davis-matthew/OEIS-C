/*
 * A246908  a(n) = sigma(n + sigma(n)) - sigma(n).
 * https://oeis.org/A246908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n + sigma(n)) - sigma(n)
 * Range:     u8 n <= 51, u16 n <= 5879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of divisors of an unsigned integer n. */
/* Uses only integer arithmetic; works for any idx_t up to uint64_t. */
static idx_t sigma_u64(idx_t n)
{
    idx_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            sum += i;                    /* i is a divisor */
            idx_t j = n / i;             /* paired divisor */
            if (j != i)
                sum += j;                /* add the paired divisor if distinct */
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* just in case; the sequence is defined for n >= 1 */
        return 0;

    idx_t sigma_n = sigma_u64(n);                /* sigma(n) as idx_t */
    idx_t sum = n + sigma_n;                     /* n + sigma(n) */
    idx_t sigma_sum = sigma_u64(sum);            /* sigma(n + sigma(n)) */

    /* The result may not fit in seq_t for very large n.  The build
       checks only terms that do fit. */
    return (seq_t)(sigma_sum - sigma_n);
}
