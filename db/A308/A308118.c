/*
 * A308118  Number of obtuse integer-sided triangles with perimeter n and
 *          squarefree sides.
 * https://oeis.org/A308118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of obtuse integer‑sided triangles with
 *            perimeter n whose three sides are squarefree
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static int8_t *mu = NULL;
static idx_t mu_cap = 0;

static void ensure_mu(idx_t n) {
    if (n <= mu_cap) return;
    idx_t size = n + 1;
    int8_t *new_mu = (int8_t *)malloc(size * sizeof(int8_t));
    if (!new_mu) exit(1);
    int *lp = (int *)malloc(size * sizeof(int));
    int *pr = (int *)malloc(size * sizeof(int));
    if (!lp || !pr) { free(new_mu); free(lp); free(pr); exit(1); }
    for (idx_t i = 0; i < size; ++i) lp[i] = 0;
    int pc = 0;
    new_mu[0] = 0;
    new_mu[1] = 1;
    for (idx_t i = 2; i < size; ++i) {
        if (!lp[i]) {
            lp[i] = (int)i;
            pr[pc++] = (int)i;
            new_mu[i] = -1;
        }
        for (int j = 0; j < pc && pr[j] <= lp[i] && (idx_t)i * pr[j] < size; ++j) {
            idx_t v = i * pr[j];
            lp[v] = pr[j];
            if (i % pr[j] == 0) {
                new_mu[v] = 0;
            } else {
                new_mu[v] = -new_mu[i];
            }
        }
    }
    free(lp);
    free(pr);
    free(mu);
    mu = new_mu;
    mu_cap = n;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    ensure_mu(n);
    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= n / 3; ++k) {
        if (mu[k] == 0) continue;
        idx_t max_i = (n - k) / 2;
        idx_t min_i = k;
        if (half > k) {
            idx_t bound = half - k + 1;
            if (bound > min_i) min_i = bound;
        }
        if (min_i > max_i) continue;
        for (idx_t i = min_i; i <= max_i; ++i) {
            if (mu[i] == 0) continue;
            idx_t j = n - k - i;
            if (mu[j] == 0) continue;
            if ((idx_t)j * j <= (idx_t)k * k + (idx_t)i * i) continue;
            count += 1;
        }
    }
    return count;
}
