/*
 * A326197  Number of divisors of n that are not reachable from n with any
 *          combination of transitions x -> gcd(x,sigma(x)) and x ->
 *          gcd(x,phi(x)).
 * https://oeis.org/A326197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are NOT reachable from n via
 *            the transitions x → gcd(x,σ(x)) or x → gcd(x,φ(x))
 * Range:     u8 n <= 725, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   divisor_count_u64, gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = d(n) – reachable(n),  where d(n)=A000005(n). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* total number of divisors */
    seq_t tau = divisor_count_u64(n);

    /* visited array: index 0 is unused */
    size_t size = (size_t)n + 1;
    unsigned char *vis = (unsigned char *)calloc(size, 1);
    if (!vis) return 0;            /* allocation failed – return 0 as a safe fallback */

    /* stack for DFS – allocate once, reuse while exploring */
    idx_t *stack = (idx_t *)malloc(size * sizeof(idx_t));
    if (!stack) {
        free(vis);
        return 0;
    }
    size_t sp = 0;                 /* stack pointer (size) */

    /* start from n itself */
    stack[sp++] = n;
    seq_t reachable = 0;

    while (sp) {
        idx_t x = stack[--sp];
        if (vis[x]) continue;
        vis[x] = 1;
        ++reachable;

        /* transition 1: gcd(x, sigma(x)) */
        seq_t sigma = sigma_of(x);
        seq_t y1 = gcd(x, sigma);
        idx_t v1 = (idx_t)y1;      /* v1 ≤ x ≤ n */
        if (!vis[v1]) stack[sp++] = v1;

        /* transition 2: gcd(x, phi(x)) */
        seq_t phi_val = phi(x);
        seq_t y2 = gcd(x, phi_val);
        idx_t v2 = (idx_t)y2;      /* v2 ≤ x ≤ n */
        if (!vis[v2]) stack[sp++] = v2;
    }

    free(stack);
    free(vis);

    return tau - reachable;
}
