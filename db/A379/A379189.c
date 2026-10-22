/*
 * A379189  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^3) * (1 - x*A(x))).
 * https://oeis.org/A379189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} * binomial(n+3*k+1,k) *
 *            binomial(2*n+k,n-k)/(n+3*k+1) * * The implementation uses the
 *            recurrence derived from the functional equation: * a(0) = 1 *
 *            a(n) = conv2[n-1] + conv4[n-1] - conv5[n-2] (n ≥ 1) * * where *
 *            conv2[m] = Σ_{i=0}^{m} a_i * a_{m-i} * conv4[m] = Σ_{i=0}^{m}
 *            con...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build */
typedef uint64_t idx_t;

static seq_t *val     = NULL;   /* a(n) values                     */
static seq_t *conv2   = NULL;   /* convolution of a with a         */
static seq_t *conv4   = NULL;   /* convolution of conv2 with conv2 */
static seq_t *conv5   = NULL;   /* convolution of conv4 with a     */
static idx_t  cap_val = 0;      /* allocated capacity for val      */
static idx_t  cap_c2  = 0;      /* allocated capacity for conv2    */
static idx_t  cap_c4  = 0;      /* allocated capacity for conv4    */
static idx_t  cap_c5  = 0;      /* allocated capacity for conv5    */
static idx_t  last    = 0;      /* largest n already computed      */

static void ensure_capacity(seq_t **arr, idx_t *cap, idx_t need, const char *name)
{
    if (need <= *cap)
        return;
    seq_t *p = (seq_t *)realloc(*arr, need * sizeof(seq_t));
    if (!p)
        abort();
    *arr = p;
    /* initialise new elements to zero */
    for (idx_t i = *cap; i < need; ++i)
        (*arr)[i] = (seq_t)0;
    *cap = need;
}

seq_t a(idx_t n)
{
    /* Need capacity for indices 0 .. n for all arrays */
    idx_t need = n + 1;
    ensure_capacity(&val,   &cap_val, need, "val");
    ensure_capacity(&conv2, &cap_c2,  need, "conv2");
    ensure_capacity(&conv4, &cap_c4,  need, "conv4");
    ensure_capacity(&conv5, &cap_c5,  need, "conv5");

    /* initialise for n==0 if necessary */
    if (last == 0 && cap_val > 0) {
        val[0]   = (seq_t)1;   /* a(0) = 1 */
        conv2[0] = (seq_t)1;   /* a0*a0   */
        conv4[0] = (seq_t)1;   /* conv2[0]*conv2[0] */
        conv5[0] = (seq_t)1;   /* conv4[0]*a0 */
        last = 0;
    }

    /* already computed? */
    if (n <= last)
        return val[n];

    /* compute terms from last+1 up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* recurrence for a(k) */
        seq_t term1 = conv2[k - 1];
        seq_t term2 = conv4[k - 1];
        seq_t term3 = (k >= 2) ? conv5[k - 2] : (seq_t)0;

        seq_t a_k = term1 + term2;
        a_k -= term3;          /* assuming a_k ≥ term3; sequence guarantees this */
        val[k] = a_k;

        /* compute conv2[k] = Σ_{i=0}^k a_i * a_{k-i} */
        seq_t c2 = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c2 += val[i] * val[k - i];
        conv2[k] = c2;

        /* compute conv4[k] = Σ_{i=0}^k conv2[i] * conv2[k-i] */
        seq_t c4 = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c4 += conv2[i] * conv2[k - i];
        conv4[k] = c4;

        /* compute conv5[k] = Σ_{i=0}^k conv4[i] * val[k-i] */
        seq_t c5 = (seq_t)0;
        for (idx_t i = 0; i <= k; ++i)
            c5 += conv4[i] * val[k - i];
        conv5[k] = c5;
    }

    last = n;
    return val[n];
}
