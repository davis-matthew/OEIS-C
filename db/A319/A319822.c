/*
 * A319822  Number of solutions to x^2 + 2*y^2 + 5*z^2 + 5*w^2 = n.
 * https://oeis.org/A319822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/5⌋} r1[n‑5k] * r2[k] where r1[m] counts
 *            integer solutions (x,y) of x²+2y²=m (signs and order included),
 *            and r2[k] counts solutions (z,w) of z²+w²=k
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static state for memoisation */
static idx_t prepared = 0;
static uint64_t *r1u = NULL;  /* array of size N+1 for x²+2y² */
static uint64_t *r2u = NULL;  /* array of size ⌊N/5⌋+1 for z²+w² */

/* build tables up to N */
static void compute_upto(idx_t N)
{
    if (N < prepared) return;          /* already computed for a larger n */

    /* free old tables */
    free(r1u);
    free(r2u);

    /* allocate new tables */
    r1u = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    r2u = (uint64_t *)calloc((size_t)(N / 5) + 1, sizeof(uint64_t));

    /* fill r1u: solutions of x² + 2y² */
    for (idx_t x = 0; (x * x) <= N; ++x) {
        idx_t xsq = x * x;
        for (idx_t y = 0;; ++y) {
            idx_t val = xsq + 2 * y * y;
            if (val > N) break;
            uint64_t mult;
            if (x == 0 && y == 0)          mult = 1;
            else if (x == 0 || y == 0)     mult = 2;
            else                            mult = 4;
            r1u[val] += mult;
        }
    }

    /* fill r2u: solutions of z² + w²  (k ≤ N/5) */
    idx_t max_k = N / 5;
    for (idx_t z = 0; (z * z) <= max_k; ++z) {
        idx_t zsq = z * z;
        for (idx_t w = 0;; ++w) {
            idx_t val = zsq + w * w;
            if (val > max_k) break;
            uint64_t mult;
            if (z == 0 && w == 0)          mult = 1;
            else if (z == 0 || w == 0)     mult = 2;
            else                            mult = 4;
            r2u[val] += mult;
        }
    }

    prepared = N;
}

seq_t a(idx_t n)
{
    compute_upto(n);
    uint64_t ans = 0;
    for (idx_t k = 0; 5 * k <= n; ++k) {
        ans += r2u[k] * r1u[n - 5 * k];
    }
    return (seq_t)ans;
}
