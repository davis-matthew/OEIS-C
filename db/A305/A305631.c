/*
 * A305631  Expansion of Product_{r not a perfect power} 1/(1 - x^r).
 * https://oeis.org/A305631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 68, u32 n <= 187, u64 n <= 571,
 *            u128 n <= 1879, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Calculated by dynamic programming: a(n) = number of partitions of n
    whose parts are not perfect powers.  */

/* Note: In C++ the type seq_t is an arbitrary‑precision class with
   overloaded operators +, -, *, /, +=, etc.  In C it will be an
   unsigned integer type; the code works in both cases.  */

#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *dp      = NULL;   /* partition counts up to current max_n */
static bool *is_pp    = NULL;   /* marks whether an integer is a perfect power */
static idx_t  max_n   = 0;

/* Helper: compute the perfect‑power flag array up to 'limit'.
   The array is allocated with calloc and stored in the static
   pointer is_pp.  Any previous array is freed. */
static void compute_perfect_powers(idx_t limit)
{
    bool *arr = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (arr == NULL)   /* allocation failure – keep old array */
        return;

    if (limit >= 1)
        arr[1] = true;          /* 1 is a perfect power (1^k) */

    /* sieve for perfect powers */
    for (idx_t base = 2; base * base <= limit; ++base)
    {
        idx_t val = base * base;
        while (val <= limit)
        {
            arr[val] = true;
            if (val > limit / base)   /* would overflow on next multiplication */
                break;
            val *= base;
        }
    }

    free(is_pp);
    is_pp = arr;
}

/* Helper: recompute the DP partition table up to 'n' using only
   integers that are NOT perfect powers.  The array is allocated
   with calloc and the static pointer dp is updated. */
static void recompute_dp(idx_t n)
{
    seq_t *newdp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (newdp == NULL)   /* allocation failure – keep old array */
        return;

    newdp[0] = (seq_t)1;          /* empty partition contributes 1 */

    for (idx_t part = 2; part <= n; ++part)
    {
        if (is_pp != NULL && is_pp[part])
            continue;                 /* skip perfect‑power parts */

        for (idx_t sum = part; sum <= n; ++sum)
            newdp[sum] += newdp[sum - part];
    }

    free(dp);
    dp     = newdp;
    max_n  = n;
}

/* The exported sequence function.  It memoises the largest 'n'
   computed so far and recomputes the tables when called with
   a larger index.  The function works for any order of calls. */
seq_t a(idx_t n)
{
    /* Base case: n == 0 */
    if (n == 0)
    {
        if (dp == NULL || max_n == 0)
            recompute_dp(0);
        return dp[0];
    }

    /* If we have not computed anything yet, or need larger tables */
    if (dp == NULL || n > max_n)
    {
        compute_perfect_powers(n);
        recompute_dp(n);
    }

    return dp[n];
}
