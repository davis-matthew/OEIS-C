/*
 * A334112  a(n) = A332809(n) - A000005(n).
 * https://oeis.org/A334112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A332809(n) - A000005(n); A332809 by DFS over k -> k -
 *            k/p, A000005 by trial division
 * Range:     u8 n <= 6250, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9933 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    bool *vis = (bool *)calloc((size_t)n + 1, sizeof(bool));
    idx_t *stack = (idx_t *)malloc(((size_t)n + 1) * sizeof(idx_t));
    if (!vis || !stack) { free(vis); free(stack); return 0; }

    idx_t top = 0, count = 1;
    stack[top++] = n;
    vis[n] = true;
    while (top) {
        idx_t x = stack[--top];
        if (x == 1) continue;
        idx_t t = x;
        for (idx_t p = 2; p <= t / p; p++) {
            if (t % p == 0) {
                while (t % p == 0) t /= p;
                idx_t y = x - x / p;
                if (!vis[y]) { vis[y] = true; count++; stack[top++] = y; }
            }
        }
        if (t > 1) {
            idx_t y = x - x / t;
            if (!vis[y]) { vis[y] = true; count++; stack[top++] = y; }
        }
    }
    free(vis);
    free(stack);

    idx_t d = 1, m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            d *= (e + 1);
        }
    }
    if (m > 1) d *= 2;

    return (seq_t)count - (seq_t)d;
}
