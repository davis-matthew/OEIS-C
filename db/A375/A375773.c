/*
 * A375773  Expansion of e.g.f. exp((exp(x) - 1)^5).
 * https://oeis.org/A375773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=120*sum_{k=1..n} binomial(n-1,k-1)*Stirling2(k,5)*a(n-k),
 *            with a(0)=1, a(n)=0 for 1≤n<5. Implementation uses a memoised
 *            recurrence and dynamic arrays
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types defined by the build */
extern seq_t a(idx_t n);

static seq_t *a_cache = NULL;
static size_t a_cap = 0;

static seq_t *S5_cache = NULL;
static size_t S5_cap = 0;

static seq_t S_prev[6];
static idx_t last_k = 0;

static void init_cache(void)
{
    if (a_cache) return;               /* already initialised */
    /* initialise a_cache[0] = 1 */
    a_cap = 1;
    a_cache = (seq_t *)malloc(sizeof(seq_t) * a_cap);
    a_cache[0] = (seq_t)1;

    /* initialise S5_cache[0] = 0 (S(0,5)=0) */
    S5_cap = 1;
    S5_cache = (seq_t *)malloc(sizeof(seq_t) * S5_cap);
    S5_cache[0] = (seq_t)0;

    /* S_prev holds S(k-1, m) for m=0..5; initialise for k=0 */
    for (int m = 0; m <= 5; ++m) S_prev[m] = (seq_t)0;
    S_prev[0] = (seq_t)1;
    last_k = 0;
}

static void expand_cache(idx_t n)
{
    if (n < a_cap) return;            /* already computed */
    size_t new_cap = (size_t)(n + 1);
    a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_cap);
    a_cap = new_cap;
    S5_cache = (seq_t *)realloc(S5_cache, sizeof(seq_t) * new_cap);
    S5_cap = new_cap;

    /* compute values for k = last_k+1 .. n */
    for (idx_t k = last_k + 1; k <= n; ++k) {
        /* compute S(k,m) for m=0..5 */
        seq_t newS[6];
        newS[0] = (seq_t)0;
        for (int m = 1; m <= 5; ++m) {
            newS[m] = (seq_t)m * S_prev[m] + S_prev[m - 1];
        }
        S5_cache[k] = newS[5];          /* Stirling2(k,5) */

        /* compute a(k) via recurrence */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* binom(k-1,0) */
        for (idx_t j = 1; j <= k; ++j) {
            if (j >= 5) {
                /* term = binom * S5_cache[j] * a_cache[k-j] */
                seq_t term = binom * S5_cache[j] * a_cache[k - j];
                sum += term;
            }
            if (j < k) {
                /* update binom = binom(k-1,j) for next j+1 */
                binom = binom * (seq_t)(k - j);
                binom = binom / (seq_t)j;
            }
        }
        a_cache[k] = (seq_t)120 * sum;

        /* prepare for next iteration */
        for (int m = 0; m <= 5; ++m) S_prev[m] = newS[m];
        last_k = k;
    }
}

seq_t a(idx_t n)
{
    init_cache();
    if (n >= a_cap) expand_cache(n);
    return a_cache[n];
}
