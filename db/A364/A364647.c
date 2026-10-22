/*
 * A364647  G.f. satisfies A(x) = 1/(1 - 5*x) - x*A(x)^3.
 * https://oeis.org/A364647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies A(x)=1/(1-5x)-x*A(x)^3 In coefficients: a(0)=1,
 *            a(n)=5ⁿ-∑_{i+j+k=n-1} a_i a_j a_k (n≥1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of computed values ------------------------------------- */
static seq_t *a_cache = NULL;
static seq_t *c_cache = NULL;
static size_t a_cap   = 0;      /* number of elements in a_cache   */
static size_t c_cap   = 0;      /* size of c_cache (convolution array)   */
static seq_t pow5 = 1;          /* 5^(a_cap-1), correct after last a computed */

/* helper to ensure convolution array size */
static void ensure_c_size(size_t need)
{
    if (c_cap < need) {
        c_cache = (seq_t*)realloc(c_cache, need * sizeof(seq_t));
        for (size_t i = c_cap; i < need; ++i)
            c_cache[i] = 0;
        c_cap = need;
    }
}

/* main sequence routine ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* already computed?  */
    if ((size_t)n < a_cap)
        return a_cache[n];

    /* grow value cache to needed size */
    a_cache = (seq_t*)realloc(a_cache, ((size_t)n + 1) * sizeof(seq_t));

    /* convolution array must be large enough for indices up to 2*n   */
    ensure_c_size(2 * (size_t)n + 1);

    /* initialise for first element if nothing yet cached */
    if (a_cap == 0) {
        a_cache[0] = (seq_t)1;    /* a(0) = 1          */
        c_cache[0] = (seq_t)1;    /* C(0)=a0*a0=1      */
        a_cap = 1;                /* now we have a[0]  */
        pow5 = (seq_t)1;          /* 5^0 = 1           */
    }

    /* compute missing terms up to n --------------------------------- */
    for (idx_t k = a_cap; k <= n; ++k) {

        /* compute b_{k-1} = Σ_{i=0}^{k-1} a_i * C(k-1-i)            */
        seq_t b = 0;
        for (idx_t i = 0; i < k; ++i)
            b += a_cache[i] * c_cache[(size_t)(k - 1 - i)];

        /* 5^k : pow5 holds 5^(k-1) at loop start, so multiply once */
        seq_t pow_k = pow5 * (seq_t)5;
        pow5 = pow_k;                     /* update for next round   */

        /* a_k = 5^k - b                                          */
        a_cache[k] = pow_k - b;

        /* update convolution array C (ordered pairs)              */
        seq_t a_k = a_cache[k];
        for (idx_t i = 0; i < k; ++i) {
            seq_t prod = a_k * a_cache[i];
            c_cache[(size_t)(k + i)] += prod;     /* (k,i) */
            c_cache[(size_t)(k + i)] += prod;     /* (i,k) */
        }
        /* pair (k,k) counted once */
        c_cache[(size_t)(2 * k)] += a_k * a_k;
    }
    a_cap = (size_t)n + 1;   /* cache now up to n          */
    return a_cache[n];
}
