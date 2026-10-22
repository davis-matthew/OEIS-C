/*
 * A375716  Expansion of e.g.f. 1 / (1 - x^3 * (exp(x) - 1))^(1/6).
 * https://oeis.org/A375716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/4⌋} (∏_{j=0}^{k-1} (6j+1)) *
 *            Stirling2(n-3k,k) / (6^k * (n-3k)!) (OEIS A375716)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

#define MAX_K 2600   /* sufficient for n up to about 10000  */

static seq_t products[MAX_K];
static seq_t pow6s[MAX_K];
static seq_t prevS[MAX_K];
static seq_t curS[MAX_K];
static seq_t neededS[MAX_K];

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0)
        return (seq_t)1;

    /* maximum possible k is ⌊n/4⌋ */
    idx_t kmax = n / 4;
    if (kmax > MAX_K - 1)
        kmax = MAX_K - 1;

    /* initialise arrays */
    for (idx_t i = 0; i <= kmax; ++i) {
        prevS[i]   = (i == 0) ? (seq_t)1 : (seq_t)0;
        curS[i]    = (seq_t)0;
        neededS[i] = (seq_t)0;
    }

    /* pre‑compute product_{j=0}^{k-1} (6j+1) and 6^k */
    products[0] = (seq_t)1;
    pow6s[0]    = (seq_t)1;
    for (idx_t k = 1; k <= kmax; ++k) {
        products[k] = products[k - 1] * (seq_t)(6 * (k - 1) + 1);
        pow6s[k]    = pow6s[k - 1] * (seq_t)6;
    }

    /* DP for Stirling numbers of the second kind.
       After trawling through m = 1..n, when m = n-3k we capture
       S(n-3k, k) into neededS[k]. */
    seq_t *pPrev = prevS;
    seq_t *pCur  = curS;

    for (idx_t m = 1; m <= n; ++m) {
        pCur[0] = (seq_t)0;
        idx_t limit = (kmax < m) ? kmax : m;
        for (idx_t k = 1; k <= limit; ++k) {
            pCur[k] = (seq_t)k * pPrev[k] + pPrev[k - 1];
        }

        /* capture S(n-3k,k) when m == n-3k */
        if ((n >= m) && ((n - m) % 3 == 0)) {
            idx_t k = (n - m) / 3;
            if (k <= kmax)
                neededS[k] = pCur[k];
        }

        /* swap rows for next m */
        seq_t *tmp = pPrev;
        pPrev = pCur;
        pCur  = tmp;
    }

    /* main summation */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t m = n - 3 * k;            /* m = n-3k  */
        if (m < k)                      /* Stirling zero if m<k */
            continue;

        seq_t s_val = neededS[k];
        if (s_val == (seq_t)0)
            continue;

        seq_t bin = binomial(n, 3 * k);           /* C(n,3k)  */
        seq_t fact_3k = factorial(3 * k);         /* (3k)!    */
        seq_t fact_3k_div6k = fact_3k / pow6s[k]; /* (3k)! / 6^k  (exact division) */

        seq_t term = bin * fact_3k_div6k;
        term *= products[k];
        term *= s_val;

        sum += term;
    }

    return sum;
}
