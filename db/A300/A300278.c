/*
 * A300278  G.f.: 1 + Sum_{n>=1} a(n)*x^n/(1 - x^n) = Product_{n>=1} (1 +
 *          n*x^n).
 * https://oeis.org/A300278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * A022629(d)
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external linkage as requested */
seq_t a(idx_t n);

/* ---------- internal cache ---------- */
static seq_t *b_arr = NULL;      /* A022629 values 0..max */
static int   *mu_arr = NULL;    /* Möbius μ values 0..max */
static idx_t  max_cache = 0;    /* last filled index */

/* ---------- helper: cache preparation ---------- */
static void ensure_cache(idx_t n)
{
    if (n <= max_cache)
        return;                     /* already prepared */

    /* Allocate new arrays (cast for C++) */
    seq_t *new_b  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    int   *new_mu = (int   *)calloc((size_t)(n + 1), sizeof(int));
    int   *lp     = (int   *)calloc((size_t)(n + 1), sizeof(int));
    uint32_t *pr  = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    size_t pr_cnt = 0;

    if (!new_b || !new_mu || !lp || !pr) {
        /* allocation failed – keep old cache unchanged */
        free(new_b); free(new_mu); free(lp); free(pr);
        return;
    }

    /* Linear sieve for μ and smallest prime factor */
    new_mu[0] = 0; new_mu[1] = 1;
    lp[0] = lp[1] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (lp[i] == 0) {               /* i is prime */
            lp[i] = (int)i;
            new_mu[i] = -1;
            pr[pr_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pr_cnt && pr[j] <= lp[i] && i * pr[j] <= n; ++j) {
            idx_t k = i * (idx_t)pr[j];
            lp[k] = (int)pr[j];
            if (i % pr[j] == 0) {
                new_mu[k] = 0;
                break;
            } else {
                new_mu[k] = -new_mu[i];
            }
        }
    }

    /* Compute A022629 values via bounded knapsack (0/1) */
    new_b[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = n; j >= k; --j) {
            new_b[j] += (seq_t)k * new_b[j - k];
        }
    }

    /* Free old cache */
    free(b_arr);
    free(mu_arr);
    free(lp);
    free(pr);

    /* Store new cache */
    b_arr = new_b;
    mu_arr = new_mu;
    max_cache = n;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */
    ensure_cache(n);

    seq_t res = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            int mu_val = mu_arr[n / d];
            if (mu_val == 1)
                res += b_arr[d];
            else if (mu_val == -1)
                res -= b_arr[d];
        }
    }
    return res;
}
