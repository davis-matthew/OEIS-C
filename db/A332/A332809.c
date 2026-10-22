/*
 * A332809  Number of distinct integers encountered on possible paths from n
 *          to 1 when iterating the nondeterministic map k -> k - k/p, where p
 *          is any of the prime factors of k.
 * https://oeis.org/A332809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of distinct integers encountered on all paths from
 *            n to 1 using the map k → k - k/p for any prime factor p of k
 * Range:     u8 n <= 6148, u16 n <= 8990, u32 n <= 8990, u64 n <= 8990,
 *            u128 n <= 8990, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for out‑of‑range calls */

    /* smallest‑prime‑factor (spf) sieve up to n */
    idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0;
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= n / i) {                /* avoid overflow of i*i          */
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* visited flag array */
    bool *vis = (bool *)calloc(n + 1, sizeof(bool));
    if (!vis) {
        free(spf);
        return 0;
    }

    /* stack for DFS */
    idx_t *stack = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!stack) {
        free(spf);
        free(vis);
        return 0;
    }

    idx_t top = 0;
    stack[top++] = n;
    vis[n] = true;
    seq_t count = 1;                 /* count includes n itself */

    /* traverse all reachable numbers */
    while (top) {
        idx_t x = stack[--top];
        if (x == 1) continue;           /* no moves from 1 */

        idx_t tmp = x;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            while (tmp % p == 0) tmp /= p;   /* skip duplicate primes */
            idx_t y = x - x / p;
            if (!vis[y]) {
                vis[y] = true;
                ++count;
                stack[top++] = y;
            }
        }
    }

    free(spf);
    free(vis);
    free(stack);
    return count;
}
