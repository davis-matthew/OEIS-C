/*
 * A308425  Sum of the perimeters of all integer-sided triangles with
 *          perimeter n whose side lengths are nonsquarefree.
 * https://oeis.org/A308425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * [number of integer‑sided triangles with perimeter n
 *            whose side lengths are nonsquarefree (i.e. μ(x)=0 for each side)
 *            ]
 * Range:     u8 n <= 51, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int8_t *mu_table = NULL;     /* μ(n) for 0 ≤ n ≤ current_limit */
static uint32_t *primes   = NULL;   /* primes for linear sieve          */
static idx_t    mu_limit  = 0;      /* highest n for which mu_table is valid */
static size_t   prime_cnt = 0;      /* number of primes stored          */

/* Compute μ(1..limit) by linear sieve */
static void compute_mobius(idx_t limit)
{
    /* Resize tables */
    mu_table = (int8_t *)realloc(mu_table, (limit + 1) * sizeof(int8_t));
    primes   = (uint32_t *)realloc(primes,   (limit + 1) * sizeof(uint32_t));
    if (mu_table == NULL || primes == NULL) return;            /* allocation failed */

    idx_t *lp = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (lp == NULL) return;

    memset(lp, 0, (limit + 1) * sizeof(idx_t));
    mu_table[0] = 0;
    mu_table[1] = 1;
    prime_cnt   = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (lp[i] == 0) {                     /* i is prime */
            lp[i]       = i;
            primes[prime_cnt++] = (uint32_t)i;
            mu_table[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t ip = i * (idx_t)p;
            if (ip > limit) break;
            lp[ip] = p;
            if (i % (idx_t)p == 0) {          /* p divides i */
                mu_table[ip] = 0;
                break;
            } else {
                mu_table[ip] = -mu_table[i];
            }
        }
    }
    free(lp);
    mu_limit = limit;
}

/* Ensure that μ for 0..n is available */
static void ensure_mu(idx_t n)
{
    if (n < mu_limit) return;
    compute_mobius(n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;              /* no triangle possible */

    ensure_mu(n);

    seq_t count = 0;                          /* number of qualifying triangles */

    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t max_i = (n - k) / 2;
        if (max_i < k) continue;
        for (idx_t i = k; i <= max_i; ++i) {
            idx_t j = n - k - i;
            if (j < i) continue;             /* enforce k ≤ i ≤ j */
            if (k + i <= j) continue;        /* triangle inequality */

            if (mu_table[k] == 0 &&
                mu_table[i] == 0 &&
                mu_table[j] == 0) {
                ++count;
            }
        }
    }

    return (seq_t)n * count;
}
