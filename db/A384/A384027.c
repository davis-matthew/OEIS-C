/*
 * A384027  a(n) = [x^(3*n)] Product_{k=0..n-1} (1 + k*x)^4.
 * https://oeis.org/A384027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(3*n) in Π_{k=0}^{n-1} (1 + k·x)^4 (OEIS
 *            A384027)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* binomial coefficients for (1 + kx)^4 */
static const unsigned int C4[5] = {1, 4, 6, 4, 1};

static seq_t *dp      = NULL;   /* current polynomial coefficients */
static seq_t *dp_next = NULL;   /* buffer for next polynomial */
static size_t dp_cap  = 0;      /* allocated capacity of dp arrays */

static idx_t cur_n = 0;         /* number of factors processed */

static seq_t *a_cache = NULL;   /* cache of a(n) values */
static size_t a_size  = 0;      /* number of cached values */

/* ensure capacity of dp arrays */
static void ensure_dp_capacity(size_t needed)
{
    if (dp_cap >= needed) return;

    seq_t *new_dp  = (seq_t*)malloc(sizeof(seq_t) * needed);
    seq_t *new_dpn = (seq_t*)malloc(sizeof(seq_t) * needed);
    if (!new_dp || !new_dpn) return; /* assume memory is available */

    /* copy existing data */
    for (size_t i = 0; i < dp_cap; ++i) new_dp[i] = dp[i];

    free(dp);
    free(dp_next);
    dp      = new_dp;
    dp_next = new_dpn;
    dp_cap  = needed;
}

/* ensure a_cache has space for at least new_size elements */
static void ensure_a_size(size_t new_size)
{
    if (a_size >= new_size) return;

    seq_t *new_a = (seq_t*)realloc(a_cache, sizeof(seq_t) * new_size);
    if (!new_a) return; /* assume memory is available */

    /* initialize newly allocated part to zero */
    for (size_t i = a_size; i < new_size; ++i) new_a[i] = 0;

    a_cache = new_a;
    a_size  = new_size;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* first call: initialise data structures for n = 0 */
    if (!a_cache) {
        a_cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (!a_cache) return 0;            /* out of memory – return 0 */
        a_cache[0] = 1;                     /* a(0) = 1 */
        a_size = 1;

        /* initialise empty product polynomial: only coefficient of degree 0 is 1 */
        ensure_dp_capacity(1);
        dp[0] = 1;
        cur_n = 0;
    }

    /* return cached value if already computed */
    if (n < a_size) return a_cache[n];

    /* extend computation up to n */
    for (idx_t target = a_size; target <= n; ++target) {
        idx_t k = cur_n;                      /* factor (1 + kx)^4 to multiply */
        idx_t old_deg = 4 * cur_n;            /* degree of current polynomial */
        idx_t new_deg = old_deg + 4;          /* degree after multiplication */

        ensure_dp_capacity(new_deg + 1);

        /* initialise next polynomial coefficients to zero */
        for (idx_t i = 0; i <= new_deg; ++i) dp_next[i] = 0;

        /* precompute powers of k: k^t for t=0..4 */
        seq_t powk[5];
        powk[0] = 1;
        for (int t = 1; t <= 4; ++t) powk[t] = powk[t-1] * (seq_t)k;

        /* convolution: dp_next[j+t] += dp[j] * C4[t] * powk[t] */
        for (idx_t j = 0; j <= old_deg; ++j) {
            seq_t base = dp[j];
            if (base == 0) continue;

            dp_next[j] += base;                 /* t = 0, C4[0] = 1, k^0 = 1 */

            for (int t = 1; t <= 4; ++t) {
                seq_t add = base * (seq_t)C4[t] * powk[t];
                dp_next[j + t] += add;
            }
        }

        /* swap dp and dp_next buffers */
        seq_t *tmp = dp; dp = dp_next; dp_next = tmp;
        cur_n++;                            /* one more factor processed */

        /* record a[target] = coefficient of x^(3*target) */
        idx_t deg_needed = 3 * target;
        seq_t val = 0;
        if (deg_needed <= 4 * target) val = dp[deg_needed];

        ensure_a_size(target + 1);
        a_cache[target] = val;
    }

    return a_cache[n];
}
