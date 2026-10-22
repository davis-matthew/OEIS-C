/*
 * A308514  Take all the integer-sided triangles with perimeter n and side
 *          lengths a, b and c such that a <= b <= c and a is prime. a(n) is
 *          the sum of all the b's.
 * https://oeis.org/A308514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all side lengths b in integer‑sided triangles *
 *            with perimeter n, side lengths a <= b <= c, a prime, and a + b >
 *            c. * a(1) = 0, a(2) = 0,
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes */

static idx_t *g_primes = NULL;   /* list of primes up to current max */
static size_t g_prime_cnt = 0;
static idx_t   g_max_limit = 0;  /* largest n/3 for which primes were built */

seq_t a(idx_t n)
{
    /* No integer‑sided triangles exist for perimeters < 3. */
    if (n < 3) return 0;

    /* All prime sides a must satisfy a <= n/3. */
    idx_t limit = n / 3;

    if (limit == 0) return 0;           /* no primes needed */

    /* Build or extend the prime list up to 'limit' if necessary. */
    if (limit > g_max_limit) {
        if (g_primes != NULL) {
            free(g_primes);
        }
        build_primes(limit, &g_primes, &g_prime_cnt);
        g_max_limit = limit;
    } else if (g_primes == NULL) {       /* first call when limit > 0 */
        build_primes(limit, &g_primes, &g_prime_cnt);
        g_max_limit = limit;
    }

    seq_t sum = 0;
    for (size_t pi = 0; pi < g_prime_cnt; ++pi) {
        idx_t a_val = g_primes[pi];
        if (a_val > limit) break;          /* safety, though shouldn't happen */

        idx_t b_min = a_val;
        idx_t b_max = (n - a_val) / 2;      /* floor((n-a)/2) */

        if (b_min > b_max) continue;

        for (idx_t b = b_min; b <= b_max; ++b) {
            /* Triangle inequality: a + b > c, with c = n - a - b. */
            if (2 * (a_val + b) <= n) continue;

            sum += (seq_t)b;
        }
    }

    return sum;
}
