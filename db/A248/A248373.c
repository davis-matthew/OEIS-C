/*
 * A248373  Partial sums of primes of form n^2 + (n+1)^2 + (n+2)^2 (A027864).
 * https://oeis.org/A248373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} prime(3*k^2 + 6*k + 5)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 18, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The partial sums of primes for the quadratic n^2+(n+1)^2+(n+2)^2. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cached sequence of partial sums */
static seq_t *sums     = NULL;   /* array of cumulative sums */
static size_t sums_cap = 0;      /* allocated capacity */
static idx_t  sums_len = 0;      /* number of stored terms */

static idx_t next_k = 0;         /* next k to test for primality */
static seq_t running_sum = 0;    /* sum of primes discovered so far */

/* Ensure capacity for at least `need` entries. */
static void ensure_capacity(size_t need)
{
    if (need <= sums_cap)
        return;
    size_t new_cap = need;
    seq_t *tmp = NULL;
    if (sums == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
    }
    if (tmp == NULL)
        exit(1);                /* allocation failure */
    sums = tmp;
    sums_cap = new_cap;
}

/* Compute the next prime of the polynomial,
   update cumulative sum, and store it. */
static void compute_next(void)
{
    for (;;) {
        seq_t kk  = (seq_t)next_k;
        seq_t val = 3 * kk * kk + 6 * kk + 5;   /* 3k^2 + 6k + 5 */

        if (is_prime_big(val)) {
            running_sum += val;
            if (sums_len == sums_cap)          /* should not happen if caller ok */
                ensure_capacity(sums_len + 1);
            sums[sums_len++] = running_sum;
            ++next_k;
            break;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; a(1) is the first term */
    if (n == 0)
        return 0;                          /* not part of the OEIS offset */

    ensure_capacity((size_t)n);
    while (sums_len < n)
        compute_next();

    return sums[n - 1];
}
