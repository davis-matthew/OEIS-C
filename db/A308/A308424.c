/*
 * A308424  Number of integer-sided triangles with perimeter n whose side
 *          lengths are nonsquarefree.
 * https://oeis.org/A308424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n whose
 *            side lengths are nonsquarefree
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global static array of Möbius values (mu[0] unused). */
static int8_t *mu = NULL;      /* mu[i] == 0 iff i is NOT squarefree   */
static idx_t mu_max = 0;       /* current maximum n for which mu is known */

/* Build the Möbius function up to n (inclusive) using a linear sieve. */
static void compute_mu_upto(idx_t n)
{
    if (n <= mu_max) return;           /* already computed */

    size_t sz = (size_t)n + 1;
    int   *spf = (int*)malloc(sz * sizeof(int));
    if (!spf) abort();
    for (size_t i = 0; i < sz; ++i) spf[i] = 0;

    /* Allocate an array for primes; enough for the range. */
    size_t pc = 0;
    size_t pc_cap = n / 10 + 1;
    int *pr = (int*)malloc(pc_cap * sizeof(int));
    if (!pr) abort();

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (int)i;
            if (pc == pc_cap) {            /* grow primes array */
                pc_cap <<= 1;
                pr = (int*)realloc(pr, pc_cap * sizeof(int));
                if (!pr) abort();
            }
            pr[pc++] = (int)i;
        }
        for (size_t j = 0; j < pc && pr[j] <= spf[i] && (idx_t)i * (idx_t)pr[j] <= n; ++j) {
            idx_t mul = (idx_t)i * (idx_t)pr[j];
            spf[mul] = pr[j];
        }
    }

    /* Compute Möbius values using the spf array. */
    int8_t *new_mu = (int8_t*)malloc(sz * sizeof(int8_t));
    if (!new_mu) abort();
    new_mu[0] = 0;                      /* unused */
    new_mu[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        int p = spf[i];
        idx_t m = i / p;
        if ((m % p) == 0) {
            new_mu[i] = 0;               /* square factor present */
        } else {
            new_mu[i] = -new_mu[m & 0xffffffffULL];
        }
    }

    free(spf);
    free(pr);

    /* Replace old mu array. */
    free(mu);
    mu = new_mu;
    mu_max = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have Möbius values up to n. */
    if (n > mu_max) compute_mu_upto(n);

    seq_t count = 0;

    /* k is the smallest side (by construction). */
    idx_t k_max = n / 3;
    for (idx_t k = 1; k <= k_max; ++k) {
        idx_t i_max = (n - k) / 2;
        for (idx_t i = k; i <= i_max; ++i) {
            idx_t l = n - i - k;                 /* third side */
            if (i + k <= l) continue;           /* not a triangle */
            if (mu[i] == 0 && mu[k] == 0 && mu[l] == 0) {
                count++;                         /* all sides nonsquarefree */
            }
        }
    }

    return count;
}
