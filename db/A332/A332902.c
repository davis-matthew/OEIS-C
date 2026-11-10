/*
 * A332902  a(1) = 1, then after the first differences of A332809.
 * https://oeis.org/A332902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = A332809(n) - A332809(n-1) for n > 1; A332809 is
 *            computed by a DFS over the map k -> k - k/p, with a
 *            smallest-prime-factor sieve
 * Range:     i8 n <= 3464, i16 n <= 10200, i32 n <= 10200, i64 n <= 10200,
 *            i128 n <= 10200, big any n
 * Validated: standard, pass, 114 terms through n = 9098 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t A332809_a(idx_t n)
{
    if (n == 0) return 0;

    idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0;
    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    bool *vis = (bool *)calloc(n + 1, sizeof(bool));
    if (!vis) {
        free(spf);
        return 0;
    }

    idx_t *stack = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!stack) {
        free(spf);
        free(vis);
        return 0;
    }

    idx_t top = 0;
    stack[top++] = n;
    vis[n] = true;
    idx_t count = 1;

    while (top) {
        idx_t x = stack[--top];
        if (x == 1) continue;

        idx_t tmp = x;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            while (tmp % p == 0) tmp /= p;
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
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    return A332809_a(n) - A332809_a(n - 1);
}
