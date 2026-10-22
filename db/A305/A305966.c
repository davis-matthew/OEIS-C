/*
 * A305966  Number of length-n restricted growth strings (RGS) with growth <=
 *          six and fixed first element.
 * https://oeis.org/A305966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * [x^(n-1)] exp( x + ∑_{j=1..6} (exp(j x) - 1)/j )
 *            Implementation uses the integer recurrence a(0) = 1 , a(1) = 1
 *            a(n) = ∑_{p=1}^{n-1} C(n-2, p-1) * t_p * a(n-p) for n ≥ 2 where
 *            t₁ = 7 and for p ≥ 2 t_p = ∑_{j=1}^6 j^(p-1). The values a(n)
 *            are stored in a static cache so successive...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- internal state ---------- */
static seq_t *a_cache = NULL;          /* cache for a(n)  */
static idx_t   a_cache_size  = 0;     /* number of values stored   */
static idx_t   a_cache_cap   = 0;     /* capacity of the cache     */

static seq_t *t_arr      = NULL;      /* t_p values   */
static idx_t   t_size    = 0;          /* highest p computed   */
static idx_t   t_cap     = 0;          /* capacity of t_arr     */
static seq_t   pow_j[6]    = {0,0,0,0,0,0};  /* j^(p-1) for j=1..6   */

static bool initialized = false;

/* ---------- helpers ---------- */
static void enlarge_t_arr(idx_t up_to)
{
    if (t_cap == 0) {
        t_cap = 8;
        t_arr = (seq_t*)malloc(t_cap * sizeof(seq_t));
    }
    if (t_size < up_to) {
        /* grow capacity if necessary */
        if (t_cap < up_to + 1) {
            size_t new_cap = t_cap * 2;
            if (new_cap < up_to + 1) new_cap = up_to + 1;
            seq_t *tmp = (seq_t*)realloc(t_arr, new_cap * sizeof(seq_t));
            t_arr = tmp;
            t_cap = (idx_t)new_cap;
        }
        /* start from where we left off */
        for (idx_t p = t_size + 1; p <= up_to; ++p) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < 6; ++j) {
                /* pow_j[j] currently holds j^(p-2); multiply by j (=j+1) to get j^(p-1) */
                seq_t mult = (seq_t)(j + 1);
                pow_j[j] *= mult;
                sum += pow_j[j];
            }
            t_arr[p] = sum;
        }
        t_size = up_to;
    }
}

static void init_state(void)
{
    /* Cache for a(n) : reserve space for a(0) and a(1) */
    a_cache_cap = 8;
    a_cache     = (seq_t*)malloc(a_cache_cap * sizeof(seq_t));
    a_cache[0]  = (seq_t)1;   /* a(0) */
    a_cache[1]  = (seq_t)1;   /* a(1) */
    a_cache_size = 2;

    /* t_p array : t[1] = 7, t[2..] computed lazily */
    t_cap = 8;
    t_arr = (seq_t*)malloc(t_cap * sizeof(seq_t));
    t_arr[0] = (seq_t)0;      /* unused  */
    t_arr[1] = (seq_t)7;      /* t_1 = 7   */
    const seq_t zero = (seq_t)0;
    for (idx_t j = 0; j < 6; ++j) pow_j[j] = (seq_t)1;  /* j^(0) = 1  */
    t_size = 1;
    initialized = true;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init_state();

    if (n == 0) return a_cache[0];
    if (n == 1) return a_cache[1];

    /* Ensure t_arr contains values up to n-1 */
    if (t_size < n - 1)
        enlarge_t_arr(n - 1);

    /* Extend a_cache to include value a(n) */
    while (a_cache_size <= n) {
        idx_t cur = a_cache_size;          /* compute a(cur) where cur ≥ 2 */

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;            /* C(cur-2, 0)  */

        /* loop over p = 1 .. cur-1  */
        for (idx_t p = 1; p <= cur - 1; ++p) {
            /* term = binom * t[p] * a[cur-p]  */
            seq_t term = binom;
            term *= t_arr[p];
            term *= a_cache[cur - p];
            sum += term;

            /* update binom for next p if needed */
            if (p < cur - 1) {
                idx_t numerator   = cur - 2 - (p - 1);   /* = cur - 1 - p  */
                seq_t new_binom   = binom;
                new_binom *= (seq_t)numerator;
                new_binom /= (seq_t)p;
                binom = new_binom;
            }
        }

        /* Store the computed a(cur) */
        if (a_cache_cap <= cur) {
            /* grow cache */
            size_t new_cap = a_cache_cap * 2;
            if (new_cap <= cur) new_cap = cur + 1;
            seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
            a_cache = tmp;
            a_cache_cap = (idx_t)new_cap;
        }
        a_cache[cur] = sum;
        ++a_cache_size;
    }

    return a_cache[n];
}
