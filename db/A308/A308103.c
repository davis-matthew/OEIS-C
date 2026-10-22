/*
 * A308103  Number of scalene triangles with perimeter n whose side lengths
 *          are squarefree.
 * https://oeis.org/A308103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of scalene triangles with perimeter n whose | *
 *            side lengths are squarefree
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int *mu_store = NULL;          /* Möbius values (0 if not squarefree) */
static idx_t mu_limit = 0;            /* largest n for which mu is computed */

/* Linear sieve to fill mu[1..n] */
static void compute_mu(idx_t n)
{
    size_t N = (size_t)n + 1;
    int *new_mu   = (int*)malloc(N * sizeof(int));
    int *is_comp  = (int*)calloc(N, sizeof(int));
    int *primes   = (int*)malloc(N * sizeof(int));   /* primes array */
    if (!new_mu || !is_comp || !primes) exit(1);

    new_mu[0] = 0;
    new_mu[1] = 1;
    size_t primes_cnt = 0;

    for (size_t i = 2; i < N; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (int)i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p   = (size_t)primes[j];
            size_t prod = i * p;
            if (prod >= N) break;
            is_comp[prod] = 1;
            if (i % p == 0) {
                new_mu[prod] = 0;
                break;
            } else {
                new_mu[prod] = -new_mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);
    if (mu_store) free(mu_store);
    mu_store = new_mu;
    mu_limit = n;
}

/* Ensure mu[1..n] is ready */
static void ensure_mu(idx_t n)
{
    if (n <= mu_limit) return;
    compute_mu(n);
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* no scalene triangle possible */

    ensure_mu(n);
    seq_t count = 0;

    /* Outer loop sets the smallest side k */
    idx_t k_max = (n - 1) / 3;
    for (idx_t k = 1; k <= k_max; ++k) {
        /* k must be squarefree */
        if (mu_store[(size_t)k] == 0) continue;

        /* i is the middle side, must be > k */
        idx_t i_max = (n - k - 1) / 2;
        for (idx_t i = k + 1; i <= i_max; ++i) {
            if (mu_store[(size_t)i] == 0) continue;

            idx_t j = n - i - k;   /* longest side, automatically > i */
            if (k + i <= j) continue;     /* triangle inequality fails */

            if (mu_store[(size_t)j] != 0) {
                ++count;
            }
        }
    }
    return count;
}
