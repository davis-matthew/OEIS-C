/*
 * A363302  E.g.f. satisfies log(A(x)) = exp(x / A(x)^3) - 1.
 * https://oeis.org/A363302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-3*n+1)^(k-1) * Stirling2(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;     /* a(n) values, indexed by n */
static seq_t *stir   = NULL;     /* current row of Stirling numbers S(n,k) */
static size_t cache_cap = 0;     /* capacity of a_cache */
static size_t stir_cap   = 0;     /* capacity of stir   */
static idx_t  current_n  = 0;    /* largest n computed so far */

/* Ensure that an array has at least 'needed' elements.
   Newly allocated part is zeroed. */
static void grow_array(seq_t **arr, size_t *cap_ptr, size_t needed)
{
    if (*cap_ptr >= needed) return;
    size_t new_cap = needed;
    seq_t *tmp = NULL;

    /* In C++ the cast is required because calloc/realloc return void*.
       In C it is still legal to cast explicitly. */
    tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!tmp) return;               /* allocation failure – let the caller fail */

    if (*arr) {
        /* Preserve existing values */
        for (size_t i = 0; i < *cap_ptr; ++i) tmp[i] = (*arr)[i];
        free(*arr);
    }
    *arr = tmp;
    *cap_ptr = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    grow_array(&a_cache, &cache_cap, (size_t)n + 1);
    grow_array(&stir,    &stir_cap,  (size_t)n + 1);

    /* initialize Stirling row 0 if we have not yet done so */
    if (current_n == 0) {
        stir[0] = (seq_t)1;         /* S(0,0) = 1 */
    }

    /* compute all rows up to n */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        /* update Stirling numbers for new row i (in place) */
        for (size_t k = (size_t)i; k > 0; --k) {
            seq_t prod = stir[k] * (seq_t)k;   /* k * S(i-1,k) */
            stir[k] = stir[k - 1] + prod;     /* S(i,k)   */
        }
        stir[0] = (seq_t)0;                 /* S(i,0) = 0 for i>0 */

        /* compute a(i) using the formula */
        seq_t base = (seq_t)((seq_t)-3 * (seq_t)i + (seq_t)1);
        seq_t pow  = (seq_t)1;              /* (-3*i+1)^(1-1) = 1 */
        seq_t sum  = (seq_t)0;
        for (size_t k = 1; k <= (size_t)i; ++k) {
            sum += pow * stir[k];
            pow  *= base;
        }
        a_cache[i] = sum;
    }

    current_n = n;
    return a_cache[n];
}
