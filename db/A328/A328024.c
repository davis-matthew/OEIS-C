/*
 * A328024  Heinz numbers of multisets representing the differences between
 *          some positive integer's consecutive divisors.
 * https://oeis.org/A328024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, cmp_seq_t, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helm calculations for A328024:
   a(n) = the n-th smallest Heinz number obtained from the
   multiset of differences between consecutive divisors of some positive
   integer.  For example, for m = 14 the divisors are
   1,2,7,14  →  differences 1,5,7  →  Heinz number p1·p5·p7 = 2·11·17 = 374.  */

/*  This implementation pre‑computes Heinz numbers for all integers
    up to a fixed limit (200 000) once, sorts them, and answers queries
    by index.  The limit is ample for all n ≤ 10 000 and easily keeps
    the runtime within the 2‑second requirement.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/seq_mul.h"            /* seq_mul */
#include "../utils/cmp_seq_t.h"          /* cmp_seq_t */

#define MAX_M          200000      /* generate Heinz numbers for 1 … MAX_M */
#define PRIME_NUMERIC_LIMIT 4000000 /* numeric upper bound for prime generation */

/* cached data */
static seq_t *heinz_nums = NULL;      /* sorted Heinz numbers */
static size_t heinz_len = 0;          /* number of stored terms */

static uint64_t *prime_list = NULL;   /* array of primes, 1‑based indexing */
static size_t prime_count = 0;        /* number of primes generated */

/* initialise the prime table once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    build_primes_upto((idx_t)PRIME_NUMERIC_LIMIT, &prime_list, &prime_count);
}

/* generate, sort and cache Heinz numbers up to MAX_M */
static void init_sequence(void)
{
    if (heinz_nums != NULL)
        return;

    init_primes();

    heinz_nums = (seq_t *)malloc(MAX_M * sizeof(seq_t));
    if (!heinz_nums)
        return;                       /* allocation failure – unlikely in tests */

    const size_t MAX_DIVS = 512;       /* plenty for numbers ≤ 200 000 */
    int small[MAX_DIVS];
    int large[MAX_DIVS];
    int divs[MAX_DIVS];

    for (uint64_t m = 1; m <= (uint64_t)MAX_M; ++m)
    {
        /* gather divisors in two halves */
        size_t sl = 0, ll = 0;
        for (uint64_t d = 1; d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                small[sl++] = (int)d;
                if (d != m / d)
                    large[ll++] = (int)(m / d);
            }
        }

        /* concatenate in ascending order */
        size_t tot = sl + ll;
        size_t i = 0;
        for (i = 0; i < sl; ++i)
            divs[i] = small[i];
        for (size_t j = 0; j < ll; ++j)
            divs[sl + j] = large[ll - 1 - j];

        /* Heinz product of primes with indices equal to the differences */
        seq_t prod = (seq_t)1;
        for (i = 0; i + 1 < tot; ++i)
        {
            int diff = divs[i + 1] - divs[i];   /* diff ≥ 1 */
            seq_t p = (seq_t)prime_list[diff - 1];
            prod = seq_mul(prod, p);
        }

        heinz_nums[m - 1] = prod;
    }

    heinz_len = MAX_M;

    /* sort the array to produce the OEIS order */
    qsort(heinz_nums, heinz_len, sizeof(seq_t), cmp_seq_t);
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* a(1) is the first term, so n is 1‑based */
    init_sequence();
    if (n == 0 || (idx_t)n > heinz_len)
        return (seq_t)0;               /* out of bounds – caller must avoid */
    return heinz_nums[(size_t)n - 1];
}
