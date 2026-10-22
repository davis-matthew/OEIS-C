/*
 * A334229  Sum of the areas of all r X s rectangles such that r + s = 2n,
 *          with r, s composite.
 * https://oeis.org/A334229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=4..n} i * (2*n-i) * (1 - c(i)) * (1 - c(2*n-i)),
 *            where c(k)=1 if k is prime, 0 otherwise
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve table.  prime_tbl[x]==0 means composite or <2, 1 means prime. */
static idx_t prime_tbl_limit = 0;
static unsigned char *prime_tbl = NULL;

/* Reallocates the sieve table to support all numbers ≤ limit.
 * The table is kept static so that repeated calls are efficient. */
static void ensure_prime_table(idx_t limit)
{
    /* We need to support numbers up to limit.  Allocate an array of
       unsigned char of size limit+1 and fill it with 1 (prime). */
    if (limit <= prime_tbl_limit)
        return;               /* already large enough */

    unsigned char *new_tbl = (unsigned char *)malloc((size_t)(limit + 1));
    if (!new_tbl)
        abort();              /* impossible in contest */

    for (idx_t i = 0; i <= limit; ++i)
        new_tbl[i] = 1;        /* assume prime */
    if (limit >= 0)
        new_tbl[0] = 0;
    if (limit >= 1)
        new_tbl[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (new_tbl[p])
            for (idx_t m = p * p; m <= limit; m += p)
                new_tbl[m] = 0;  /* composite */

    free(prime_tbl);
    prime_tbl = new_tbl;
    prime_tbl_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    /* Need primes up to 2*n */
    ensure_prime_table(2 * n);

    seq_t sum = 0;
    idx_t two_n = 2 * n;
    for (idx_t i = 4; i <= n; ++i)
    {
        idx_t j = two_n - i;          /* j = 2*n - i  */
        if (!prime_tbl[i] && !prime_tbl[j])  /* both composite */
            sum += (seq_t)i * (seq_t)j;
    }
    return sum;
}
