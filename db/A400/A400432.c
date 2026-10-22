/*
 * A400432  G.f. A(x) satisfies A(x) = 1 + 3*x*A(3*x)^(1/3).
 * https://oeis.org/A400432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400432
   a(0) = 1
   a(1) = 3
   a(n) = (1/27) * (3^(n+2)*a(n-1) - Σ_{1≤i,j,k≤n-1, i+j+k=n+2} a(i)a(j)a(k))   for n≥2
   Implementation uses dynamic programming; the triple sum is evaluated by a
   convolution of two first‑order convolutions. */

#include <stdlib.h>
#include <string.h>

/* External integer types */
typedef uint64_t idx_t;

static seq_t *a_cache   = NULL;   /* a[0 .. max_n]            */
static size_t  a_cap    = 0;
static seq_t *conv2     = NULL;   /* pairwise convolution N>1 */
static size_t  conv2_cap= 0;
static idx_t   max_n    = SIZE_MAX; /* not yet computed          */
static seq_t   pow3     = 27;      /* holds 3^(current+n+2)   */

/* Ensure the cache holds at least indices [0..n]   */
static void ensure_cache(idx_t n)
{
    if (a_cap > n) return;
    size_t new_cap = (n + 1) * 2;
    a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure the pairwise convolution array holds indices up to [0..need]   */
static void ensure_conv2(idx_t need)
{
    if (conv2_cap > need) return;
    size_t new_cap = (need + 1) * 2;
    seq_t *new_ptr = (seq_t*)realloc(conv2, new_cap * sizeof(seq_t));
    /* Zero new part */
    memset(new_ptr + conv2_cap, 0, (new_cap - conv2_cap) * sizeof(seq_t));
    conv2 = new_ptr;
    conv2_cap = new_cap;
}

/* Main function: returns a(n) for any nonnegative n, memoised on subsequent calls. */
seq_t a(idx_t n)
{
    /* Initialise for first call */
    if (max_n == SIZE_MAX) {
        /* a[0] and a[1] */
        ensure_cache(1);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        max_n = 1;
        /* Initialise pairwise convolution: only pair (1,1) is available
           conv2[2] = a[1]*a[1] = 9 */
        ensure_conv2(4);
        conv2[2] = a_cache[1] * a_cache[1];
        conv2[3] = (seq_t)0;
        conv2[4] = (seq_t)0;
    }

    /* Extend the cache up to the requested n */
    if (n > max_n) {
        /* Ensure arrays big enough for new indices and for pairwise sums */
        ensure_cache(n);
        ensure_conv2(3 * n + 5);

        /* Compute values for indices max_n+1 .. n */
        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* Update power of three: pow3 becomes 3^(cur+2) */
            pow3 = pow3 * 3;

            /* Compute the triple sum S = Σ a(k)*conv2[cur+2-k] */
            seq_t S = 0;
            for (idx_t k = 1; k < cur; ++k) {
                idx_t t = (cur + 2) - k;            /* t ranges from cur+1 down to 3   */
                S = S + a_cache[(size_t)k] * conv2[(size_t)t];
            }

            /* Compute a[cur] via the recurrence */
            seq_t numerator   = pow3 * a_cache[(size_t)(cur - 1)] - S;
            a_cache[(size_t)cur] = numerator / 27;

            /* Update the pairwise convolution array with the new index cur */
            for (idx_t i = 1; i <= cur; ++i) {
                seq_t prod = a_cache[(size_t)cur] * a_cache[(size_t)i];
                idx_t t = cur + i;
                conv2[(size_t)t] = conv2[(size_t)t] + prod;
                if (i != cur) {
                    conv2[(size_t)t] = conv2[(size_t)t] + prod;   /* second ordering */
                }
            }
        }
        max_n = n;
    }

    return a_cache[(size_t)n];
}
