/*
 * A305203  Expansion of e.g.f. Product_{k>=1} (1 + H(k)*x^k), where H(k) is
 *          the k-th harmonic number.
 * https://oeis.org/A305203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * coefficient of x^n in * Product_{k>=1} (1 +
 *            H(k)*x^k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  H(k) = harmonic number k = sum_{i=1}^k 1/i                */

#include <stddef.h>
#include <stdint.h>
#if defined(__cplusplus)
#  include <cstdlib>
#else
#  include <stdlib.h>
#endif

/* Helper to allocate an array of seq_t, initialized to zero */
#if defined(__cplusplus)
static seq_t* alloc_seq_array(size_t sz) { return new seq_t[sz](); }
static void free_seq_array(seq_t *p)       { delete[] p; }
#else
static seq_t* alloc_seq_array(size_t sz) { return (seq_t*)calloc(sz, sizeof(seq_t)); }
static void free_seq_array(seq_t *p)       { free(p); }
#endif

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache ------------------------------------------------------- */
    static seq_t *cache = NULL;      /* cached a[ ] values          */
    static size_t cached_size = 0;   /* size of cache               */

    /* If value already computed, return it -------------------------------- */
    if (cached_size > 0 && n < cached_size)        /* first element is a(0)   */
        return cache[n];

    /* Re‑compute all values up to n --------------------------------------- */
    /* Allocate temporary arrays ------------------------------------------ */
    size_t sz = n + 1;
    seq_t *fac = alloc_seq_array(sz);   /* factorials                 */
    seq_t *w   = alloc_seq_array(sz);   /* w(k) = H(k)*k!             */
    seq_t *a   = alloc_seq_array(sz);   /* temporary a for calculation*/

    /* Compute factorials                                                */
    fac[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fac[i] = fac[i - 1] * (idx_t)i;    /* safe: (i) converts to seq_t */

    /* Compute w[1..n]                                                */
    w[0] = (seq_t)0;                      /* unused */
    if (n >= 1) w[1] = (seq_t)1;           /* w(1) = 1                      */
    for (idx_t k = 2; k <= n; ++k)
        w[k] = w[k - 1] * (idx_t)k + fac[k - 1];

    /* Initialize a[0] = 1, others 0                                    */
    a[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) a[i] = (seq_t)0;

    /* Main double loop --------------------------------------------------- */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t wk = w[k];

        /* initial binom (n choose k) */
        seq_t binom = (seq_t)1;
        for (idx_t i = 1; i <= k; ++i)
            binom = binom * ( (idx_t)(n - k + i) ) / (idx_t)i;

        /* inner loop: m from n down to k                                 */
        for (idx_t m = n; m >= k; --m) {
            /* Skip if previous a[m-k] == 0 to avoid needless work       */
            if (a[m - k] != (seq_t)0) {
                seq_t tmp = binom;
                tmp = tmp * wk;
                tmp = tmp * a[m - k];
                a[m] += tmp;
            }
            /* update binom for next m (m-1)                      */
            if (m > k) {
                binom = binom * ( (idx_t)(m - k) ) / (idx_t)m;
            }
        }
    }

    /* Free temporary arrays --------------------------------------------- */
    free_seq_array(fac);
    free_seq_array(w);

    /* Update global cache ----------------------------------------------- */
    seq_t *new_cache = alloc_seq_array(sz);
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = a[i];
    free_seq_array(a);

    free_seq_array(cache); /* discard old cache if any */
    cache = new_cache;
    cached_size = sz;      /* cache now holds a[0]..a[n] */

    return cache[n];
}
