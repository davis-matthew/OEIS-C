/*
 * A294352  Product of first n terms of the binomial transform of the
 *          factorial.
 * https://oeis.org/A294352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294352:
   a(n) = Π_{i=1}^{n} b(i),
   where b(i) = Σ_{k=0}^{i} binom(i,k)·k!   (binomial transform of factorial)
   Offset: 0
*/
#include <stdlib.h>

static idx_t cache_n = 0;                 /* largest n computed so far */
static seq_t *fact_arr = NULL;            /* factorials fact_arr[i] = i!   */
static size_t fact_cap = 0;
static seq_t *b_arr = NULL;               /* b-values b_arr[i] = b(i)      */
static size_t b_cap = 0;
static seq_t *a_arr = NULL;               /* cumulative products a_arr[i]  */
static size_t a_cap = 0;

/* Ensure an array has room for at least 'need' elements */
static void ensure_capacity(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 1;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Expand cached data up to the requested n */
    if (cache_n < n) {
        idx_t start = cache_n + 1;
        idx_t end   = n;

        /* Allocate/resize arrays */
        ensure_capacity(&fact_cap, &fact_arr, end + 1);
        ensure_capacity(&b_cap,   &b_arr,   end + 1);
        ensure_capacity(&a_cap,   &a_arr,   end + 1);

        /* Initialize base case if this is the first call */
        if (cache_n == 0) {
            fact_arr[0] = (seq_t)1;
            a_arr[0]    = (seq_t)1;   /* product of zero terms */
        }

        for (idx_t i = start; i <= end; ++i) {
            /* Compute factorial i! */
            fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

            /* Compute b(i) = Σ_{k=0}^{i} binom(i,k) * k! */
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;               /* binom(i,0) */
            sum += binom * fact_arr[0];           /* term for k=0 */

            for (idx_t k = 1; k <= i; ++k) {
                /* binom(i,k) from binom(i,k-1) */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
                sum += binom * fact_arr[k];
            }
            b_arr[i] = sum;

            /* Compute a(i) = a(i-1) * b(i) */
            a_arr[i] = a_arr[i - 1] * sum;
        }

        cache_n = n;
    }

    return a_arr[n];
}
