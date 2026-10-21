/*
 * A259401  a(n) = Sum_{k=0..n} 2^(n-k)*p(k), where p(k) is the partition
 *          function A000041.
 * https://oeis.org/A259401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * p(k) (p(k) = partition number)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Forward declaration of the sequence function */
static void ensure_cache(idx_t n);

static seq_t *p_arr = NULL;   /* partition numbers p[0..n] */
static seq_t *a_arr = NULL;   /* sequence values a[0..n] */
static idx_t  max_cached = 0; /* highest index for which arrays are valid */

static void ensure_cache(idx_t n)
{
    /* Initial allocation for index 0 */
    if (p_arr == NULL) {
        /* Allocate single element arrays and set base values */
#if defined(__cplusplus)
        p_arr = new seq_t[1];
        a_arr = new seq_t[1];
#else
        p_arr = (seq_t *)malloc(sizeof(seq_t));
        a_arr = (seq_t *)malloc(sizeof(seq_t));
        if (!p_arr || !a_arr) abort();
#endif
        p_arr[0] = (seq_t)1;   /* p(0) = 1 */
        a_arr[0] = (seq_t)1;   /* a(0) = 1 */
        max_cached = 0;

        if (n == 0)
            return;
    }

    if (n <= max_cached)
        return; /* already computed up to this index */

    idx_t old_size = max_cached + 1;
    idx_t new_size = n + 1;
    seq_t *newp;
    seq_t *newa;

#if defined(__cplusplus)
    /* Allocate new arrays and copy old values */
    newp = new seq_t[new_size];
    newa = new seq_t[new_size];
    for (idx_t i = 0; i < old_size; ++i) {
        newp[i] = p_arr[i];
        newa[i] = a_arr[i];
    }
#else
    /* Use realloc for C builds */
    newp = (seq_t *)realloc(p_arr, new_size * sizeof(seq_t));
    newa = (seq_t *)realloc(a_arr, new_size * sizeof(seq_t));
    if (!newp || !newa) abort();
#endif

    /* Compute partition numbers and sequence values from old_size to n */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t m = 1; ; ++m) {
            idx_t pent1 = (m * (3 * m - 1)) / 2;
            if (pent1 > i) break;
            if ((m & 1) == 1)
                pos += newp[i - pent1];
            else
                neg += newp[i - pent1];

            idx_t pent2 = (m * (3 * m + 1)) / 2;
            if (pent2 > i) continue;
            if ((m & 1) == 1)
                pos += newp[i - pent2];
            else
                neg += newp[i - pent2];
        }
        newp[i] = pos - neg;          /* p(i) */
        newa[i] = newa[i - 1] * ((seq_t)2) + newp[i]; /* a(i) = 2*a(i-1) + p(i) */
    }

    /* Release old memory */
#if defined(__cplusplus)
    delete[] p_arr;
    delete[] a_arr;
#endif

    p_arr = newp;
    a_arr = newa;
    max_cached = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_arr[n];
}
