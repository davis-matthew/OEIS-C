/*
 * A308428  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n whose sides are nonsquarefree.
 * https://oeis.org/A308428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (expanded form):
   a(n) = Σ_{k=1}^{⌊n/3⌋} Σ_{i=k}^{⌊(n−k)/2⌋}
            1_{k, i, n−k−i are nonsquarefree} · (n − k − i)
   (The sign and μ‑powers in the OEIS definition simply enforce that the
   triangle inequalities hold and that all sides are not squarefree.) */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* --- Möbius function cache -------------------------------------------- */
static int *mu_cache = NULL;       /* μ(x) values for 0 … current_limit−1   */
static idx_t mu_cache_limit = 0;   /* number of stored values (always ≥ 1) */

/* Compute μ(x) for 0 … N and store them in mu_cache (replacing any old cache). */
static void compute_mu_upto(idx_t N)
{
    /* Allocate new cache */
    int *mu = (int *)malloc((N + 1) * sizeof(int));
    if (!mu) return;                       /* out of memory – silently fail */

    /* Smallest prime factor sieve */
    int *spf = (int *)malloc((N + 1) * sizeof(int));
    if (!spf) { free(mu); return; }

    for (idx_t i = 0; i <= N; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!spf[i]) {                     /* i is prime          */
            for (idx_t j = i; j <= N; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }

    mu[0] = 0; mu[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        int p = spf[i];
        int x = i / p;
        if (x % p == 0)            /* square factor */
            mu[i] = 0;
        else
            mu[i] = -mu[x];
    }

    free(spf);

    /* Replace old cache */
    free(mu_cache);
    mu_cache = mu;
    mu_cache_limit = N + 1;            /* next free index is N+1 */
}

/* Ensure that μ values are available up to at least n. */
static void ensure_mu(idx_t n)
{
    if (n + 1 <= mu_cache_limit) return;
    compute_mu_upto(n);
}

/* --- Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* no triangle is possible */

    ensure_mu(n);                        /* guarantees mu_cache[n] exists */

    seq_t total = 0;

    for (idx_t k = 1; k <= n / 3; ++k) {
        if (mu_cache[k] != 0) continue;   /* must be nonsquarefree (μ=0) */

        for (idx_t i = k; i <= (n - k) / 2; ++i) {
            if (mu_cache[i] != 0) continue;   /* nonsquarefree     */

            idx_t j = n - k - i;              /* third side       */
            if (k + i <= j) continue;          /* triangle inequality fail */

            if (mu_cache[j] != 0) continue;   /* nonsquarefree     */

            total += j;                       /* add largest side  */
        }
    }

    return total;
}
