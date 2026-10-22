/*
 * A339150  a(n) = Sum_{k=1..n} floor(k/2)! * floor((n - k)/2)!
 *          binomial((n-floor(k/2)-1), n-k).
 * https://oeis.org/A339150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(k/2)! * floor((n - k)/2)! *
 *            binomial((n - floor(k/2) - 1), n - k)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1, external linkage as requested */

#include <stdlib.h>

static seq_t *fact_cache = NULL;      /* factorials 0..fact_size-1 */
static size_t fact_size = 0;          /* number of elements in fact_cache */

/* Ensure factorials up to n_half (inclusive) are cached */
static void ensure_factorials(idx_t n_half)
{
    if ((size_t)n_half < fact_size) return;

    size_t new_size = (size_t)n_half + 1;
    seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    fact_cache = tmp;

    size_t i = fact_size;
    if (i == 0) {
        fact_cache[0] = (seq_t)1;
        ++i;
    }
    for (; i < new_size; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0 */

    /* Precompute factorials up to floor(n/2) */
    idx_t n_half = n / 2;
    ensure_factorials(n_half);

    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;            /* C(a, m) for current k */
    idx_t a_val = n - 1;               /* a = n - floor(k/2) - 1, for k=1 */
    idx_t m_val = n - 1;               /* m = n - k, for k=1 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t u = k / 2;                 /* floor(k/2) */
        idx_t v = (n - k) / 2;           /* floor((n-k)/2) */

        seq_t term = fact_cache[u];
        term *= fact_cache[v];
        term *= binom;
        sum += term;

        if (k == n) break;   /* last iteration */

        /* prepare next values of a and m */
        idx_t a_next = n - ((k + 1) / 2) - 1;
        idx_t m_next = n - (k + 1);

        if (a_next == a_val) {
            /* a stays the same, m decreases by 1:
               C(a, m-1) = C(a, m) * m / (a - m + 1) */
            seq_t denom = (seq_t)(a_val - m_val + 1);
            binom = (binom * (seq_t)m_val) / denom;
        } else {
            /* a decreases by 1 (k was odd), m decreases by 1:
               C(a-1, m-1) = C(a, m) * m / a */
            binom = (binom * (seq_t)m_val) / (seq_t)a_val;
        }

        a_val = a_next;
        m_val = m_next;
    }

    return sum;
}
