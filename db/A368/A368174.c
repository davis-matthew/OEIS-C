/*
 * A368174  Expansion of e.g.f. -log(1 - x^3/6 * (exp(x) - 1)).
 * https://oeis.org/A368174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,3) + Σ_{k=4..n-1} binomial(k,3) *
 *            binomial(n-1,k) * a(n-k) (a(0)=a(1)=a(2)=a(3)=0)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t *a_cache      = NULL;   /* memoised values of a(n)          */
static size_t cache_cap    = 0;       /* allocated capacity (≥ n+1)       */
static idx_t  computed_len = 0;       /* first index not yet computed     */

/* Compute n choose 3; returns 0 for n<3.  Uses seq_t arithmetic. */
static inline seq_t binom3(idx_t n)
{
    if (n < 3) return (seq_t)0;
    return (seq_t)n * (n - 1) * (n - 2) / 6;
}

/* Ensure that a_cache can hold indices 0 … n.  Uses seq_realloc to keep
   construction behavior of an arbitrary‑precision seq_t. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap) return;

    /* realloc will default‑construct new elements for arbitrary‑precision types. */
    seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!tmp) abort();                           /* out of memory */
    a_cache      = tmp;
    cache_cap    = (size_t)(n + 1);
}

/* Compute a(n) for all n up to the requested value. */
static void compute_up_to(idx_t n)
{
    if ((size_t)(n + 1) <= computed_len) return;

    ensure_capacity(n);

    for (idx_t i = computed_len; i <= n; ++i)
    {
        if (i < 4) {                     /* a(0)=a(1)=a(2)=a(3)=0 */
            a_cache[i] = (seq_t)0;
            continue;
        }

        seq_t sum = binom3(i);           /* binomial(i,3) */

        /* Binomial(n-1,k) initialized for k=4.  Updated incrementally. */
        seq_t binom_n1_k = binomial(i - 1, 4);

        for (idx_t k = 4; k <= i - 1; ++k)
        {
            idx_t m = i - k;             /* earlier term a(m) */
            seq_t term = binom3(k) * binom_n1_k * a_cache[m];
            sum += term;

            /* Update binomial(i-1, k+1) if not at last iteration. */
            if (k < i - 1)
                binom_n1_k = binom_n1_k * (i - 1 - k) / (k + 1);
        }
        a_cache[i] = sum;
    }

    computed_len = n + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
