/*
 * A288785  Number of blocks of size >= three in all set partitions of n.
 * https://oeis.org/A288785
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n+1) - Σ_{j=0..2} C(n,j) * Bell(n-j) (offset 3)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised array of Bell numbers.
   bell[0] = Bell(0) = 1. */
static seq_t *bell = NULL;   /* array of Bell numbers */
static idx_t bell_cap = 0;   /* allocated capacity (number of elements) */
static idx_t bell_size = 0;  /* number of Bell numbers actually computed
                                (indices 0 .. bell_size-1 are available) */

/* Grow the bell array to hold all Bell numbers up to index N (inclusive).
   The array is allocated with a capacity that is a power of two
   and larger than or equal to N+1. */
static void ensure_bell_upto(idx_t N)
{
    /* All needed values already computed? */
    if (N < bell_size)
        return;

    /* Grow or create the array */
    if (bell == NULL) {                     /* first time */
        idx_t cap = 1;
        while (cap <= N)
            cap <<= 1;                     /* at least N+1 elements */
        idx_t sz = cap;
#ifdef __cplusplus
        bell = new seq_t[sz];
#else
        bell = (seq_t *)calloc((size_t)sz, sizeof(seq_t));
#endif
        bell_cap = sz;
        bell_size = 0;
    } else if (bell_cap < N + 1) {           /* need more capacity */
        idx_t new_cap = bell_cap == 0 ? 1 : bell_cap;
        while (new_cap <= N)
            new_cap <<= 1;
        idx_t sz = new_cap;
        seq_t *new_arr;
#ifdef __cplusplus
        new_arr = new seq_t[sz];
#else
        new_arr = (seq_t *)calloc((size_t)sz, sizeof(seq_t));
#endif
        for (idx_t i = 0; i < bell_size; ++i)
            new_arr[i] = bell[i];
#ifdef __cplusplus
        delete[] bell;
#else
        free(bell);
#endif
        bell = new_arr;
        bell_cap = sz;
    }

    /* Ensure Bell(0) = 1 */
    if (bell_size == 0) {
        bell[0] = (seq_t)1;
        bell_size = 1;
    }

    /* Compute missing Bell numbers from bell_size up to N */
    for (idx_t m = bell_size; m <= N; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;           /* C(m-1,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += binom * bell[k];
            if (k + 1 < m) {
                binom = binom * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
            }
        }
        bell[m] = sum;
    }
    bell_size = N + 1;      /* indices 0 .. N are now ready */
}

/* Sequence element a(n) – number of blocks of size ≥ 3 in all set
   partitions of n (offset 3).  Returns 0 for n < 3. */
seq_t a(idx_t n)
{
    if (n < (idx_t)3)
        return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Need Bell numbers up to n+1 and lower indices. */
    ensure_bell_upto(n + 1);

    seq_t res = bell[n + 1];            /* Bell(n+1) */

    /* Subtract C(n,0)*Bell(n) = Bell(n) */
    res -= bell[n];

    /* Subtract C(n,1)*Bell(n-1) = n * Bell(n-1) */
    res -= (seq_t)n * bell[n - 1];

    /* Subtract C(n,2)*Bell(n-2) = n*(n-1)/2 * Bell(n-2) */
    seq_t t = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2;
    t *= bell[n - 2];
    res -= t;

    return res;
}
