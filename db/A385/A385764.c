/*
 * A385764  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x) - x^5*A''''(x)).
 * https://oeis.org/A385764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} (1‑6k+11k²‑6k³+k⁴) * a(k) * a(n‑1‑k) a(0) =
 *            1 a(1) = 1 a(2) = 2 a(3) = 5 a(4) = 14 a(5) = 378 a(6) = 46500
 *            a(7) = 16879869 a(8) = 14229776750 a(9) = 23948731244678 a(10) =
 *            72492823741526156 a(11) = 365581334105823084634 a(12) =
 *            2896500982661242290253612 a(13) = 34419121542689...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;      /* memoized values a(n) */
static size_t memo_len = 0;     /* number of valid terms stored */
static size_t memo_cap = 0;     /* allocated capacity for memo */

/* (k^2-3k+1)^2 = 1‑6k+11k²‑6k³+k⁴  --> pre‑computed coefficients */
static seq_t *coef = NULL;     /* coefficients for k = 0 .. */
static size_t coef_len = 0;
static size_t coef_cap = 0;

/* grow an array to hold at least 'need' elements */
static void *grow(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < need) new_cap <<= 1;
        void *new_ptr = realloc(ptr, new_cap * elem_size);
        if (!new_ptr) return NULL; /* allocation failure */
        ptr    = new_ptr;
        *cap   = new_cap;
    }
    return ptr;
}

/* ensure memo contains terms up to n */
static void compute_to(idx_t n)
{
    /* compute memo and coefficient arrays up to n */
    for (size_t i = memo_len; i <= (size_t)n; ++i) {
        if (i == 0) {            /* a(0) already set below */
            memo[i] = (seq_t)1;
            coef[i] = (seq_t)1;  /* (0^2‑0+1)^2 = 1 */
            continue;
        }

        /* ensure coefficient for k = i-1 exists */
        if (coef_len <= (size_t)i) {
            coef_len = i;
        }

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t k_seq = (seq_t)k;
            /* compute poly = k^2 - 3k + 1 */
            seq_t poly = k_seq * k_seq - (seq_t)3 * k_seq + (seq_t)1;
            seq_t coeff = poly * poly;                 /* (poly)^2 */
            seq_t prod = coeff * memo[(size_t)k] * memo[(size_t)(i - 1 - k)];
            sum += prod;
        }
        memo[(size_t)i] = sum;
    }
    memo_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!memo) {
        memo     = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!memo) return (seq_t)0;
        memo_cap = 16;
        memo_len = 0;
    }
    if (!coef) {
        coef     = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!coef) return (seq_t)0;
        coef_cap = 16;
        coef_len = 0;
    }

    /* ensure capacity for memo and coef */
    if (memo_cap < (size_t)n + 1)
        memo = (seq_t *)grow(memo, &memo_cap, (size_t)n + 1, sizeof(seq_t));
    if (coef_cap < (size_t)n)
        coef = (seq_t *)grow(coef, &coef_cap, (size_t)n, sizeof(seq_t));
    if (!memo || !coef) return (seq_t)0;  /* allocation failure */

    /* compute missing terms if needed (calls may come in any order) */
    if (memo_len <= (size_t)n) {
        /* set a(0)=1 and coefficient for k=0 */
        memo[0]  = (seq_t)1;
        coef[0] = (seq_t)1;
        compute_to(n);
    }

    return memo[(size_t)n];
}
