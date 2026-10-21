/*
 * A294103  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two squarefree parts or two nonsquarefree
 *          parts.
 * https://oeis.org/A294103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i=1..n} (n-i) * [is_squarefree(i) ==
 *            is_squarefree(2n-i)]
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* integer floor(sqrt(n)) */
static idx_t floor_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* cache of square‑free flags */
static uint8_t *sqfree = NULL;   /* sqfree[x] == 1  ⇔  x is square‑free   */
static idx_t   sqfree_limit = 0;

/* build square‑free table up to limit (inclusive)   */
static void build_sqfree(idx_t limit)
{
    if (limit <= sqfree_limit) return;          /* already ok */

    /* previous table free */
    if (sqfree) free(sqfree);

    sqfree = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!sqfree) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) sqfree[i] = 1;   /* assume square‑free */

    idx_t sqrt_lim = floor_isqrt(limit);

    /* sieve of primes up to sqrt_lim */
    uint8_t *is_prime = (uint8_t*)malloc((sqrt_lim + 1) * sizeof(uint8_t));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= sqrt_lim; ++i) is_prime[i] = 1;
    is_prime[0] = 0;
    if (sqrt_lim >= 1) is_prime[1] = 0;
    for (idx_t p = 2; p * p <= sqrt_lim; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= sqrt_lim; j += p)
                is_prime[j] = 0;

    /* mark numbers divisible by a square > 1 as non‑square‑free */
    for (idx_t p = 2; p <= sqrt_lim; ++p)
        if (is_prime[p]) {
            idx_t sq = p * p;
            for (idx_t m = sq; m <= limit; m += sq)
                sqfree[m] = 0;
        }

    free(is_prime);
    sqfree_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused – offset is 1 */

    idx_t limit = n * 2;            /* 2n */
    build_sqfree(limit);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (sqfree[i] == sqfree[limit - i])
            sum += (seq_t)(n - i);

    sum <<= 1;   /* multiply by 2 */
    return sum;
}
