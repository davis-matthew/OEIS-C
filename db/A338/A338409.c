/*
 * A338409  a(n) is the number of nodes with depth of n in a binary tree
 *          defined as: root = 1 and a child (C) of a node (N) is such that
 *          A338215(C) = N. For nodes with two children, the smaller child is
 *          assigned as the left child and the bigger one as the right child.
 *          A child of a one-child node is assigned...
 * https://oeis.org/A338409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   level sets are intervals [L(d), L(d+1)-1] with L(0)=1, L(1)=2,
 *            L(d+1) = least m with f(m) >= L(d), where f(m) = A338215(m) = m
 *            - pi(m) + pi(m - pi(m)); a(n) = L(n+1) - L(n), a(0) = 1.
 *            Computed by a sieve scan
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    uint64_t limit = 256;
    for (;;) {
        uint32_t *pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
        unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
        if (!pi || !comp) { free(pi); free(comp); return (seq_t)0; }
        uint64_t i, j;
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
        pi[0] = 0; pi[1] = 0;
        for (i = 2; i <= limit; i++) pi[i] = pi[i - 1] + (comp[i] ? 0 : 1);

        uint64_t prev = 1, cur = 2, m = 2;
        int ok = 1;
        uint64_t d;
        for (d = 1; d <= (uint64_t)n; d++) {
            uint64_t target = cur;
            if (m < target) m = target;
            for (;;) {
                if (m > limit) { ok = 0; break; }
                uint64_t f = m - pi[m] + pi[m - pi[m]];
                if (f >= target) break;
                m++;
            }
            if (!ok) break;
            prev = cur;
            cur = m;
        }
        free(pi);
        free(comp);
        if (ok) return (seq_t)(cur - prev);
        limit *= 2;
    }
}
