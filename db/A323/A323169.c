/*
 * A323169  Greatest common divisor of the set of solutions to phi(x) = n, or
 *          0 if n is not a totient.
 * https://oeis.org/A323169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD{ x | φ(x) = n } (0 if φ(x) = n has no solution)
 * Range:     u8 n <= 91, u16 n <= 9667, u32 n <= 9667, u64 n <= 9667,
 *            u128 n <= 9667, big any n
 * Helpers:   gcd, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes totients up to a fixed bound
 * and accumulates the gcd of the inverse images for each n.
 * The precomputation is performed only once on the first call. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/seq_alloc.h"

static seq_t *gcd_table = NULL;
static int initialized = 0;
static const idx_t N_MAX = 200000;   /* maximum n we precompute for */
static const idx_t M   = 200000;     /* max x for which φ(x) is computed */

/* One‑time initialization: compute φ(x) for x ≤ M and update gcd_table */
static void init_once(void)
{
    if (initialized) return;

    /* Allocate table for gcd values.  It is zero‑initialised. */
    gcd_table = seq_alloc(N_MAX + 1);
    for (idx_t i = 0; i <= N_MAX; ++i) gcd_table[i] = 0;

    /* Compute φ(1)…φ(M) with a classic sieve.  φ is stored in a uint64_t array. */
    uint64_t *phi = (uint64_t *)malloc((M + 1) * sizeof(uint64_t));
    for (idx_t i = 0; i <= M; ++i) phi[i] = i;
    for (idx_t p = 2; p <= M; ++p)
    {
        if (phi[p] == p)                /* p is prime */
        {
            for (idx_t k = p; k <= M; k += p)
            {
                phi[k] -= phi[k] / p;   /* φ(k) -= φ(k)/p */
            }
        }
    }

    /* For every x with φ(x)=t, accumulate gcd(t) = gcd(previous, x). */
    for (idx_t x = 1; x <= M; ++x)
    {
        idx_t t = (idx_t)phi[x];
        if (t > N_MAX) continue;        /* ignore too large totient values */

        seq_t sx = (seq_t)x;
        if (gcd_table[t] == 0)          /* first solution found */
            gcd_table[t] = sx;
        else
            gcd_table[t] = gcd(gcd_table[t], sx);
    }

    free(phi);
    initialized = 1;
}

/* Public function required by the OEIS framework. */
seq_t a(idx_t n)
{
    init_once();
    if (n > N_MAX) return 0;
    return gcd_table[n];
}
