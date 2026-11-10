/*
 * A307002  Numbers > 1 not of the form (3n*k - A319929(n,k))/2 where n and k
 *          > 1.
 * https://oeis.org/A307002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer m>1 not of the form (3*i*j -
 *            A319929(i,j))/2, i,j>1; found by sieve with doubling bound
 * Range:     u8 n <= 91, u16 n <= 4399, u32 n <= 4399, u64 n <= 4399,
 *            u128 n <= 4399, big any n
 * Validated: standard, pass, 77 terms through n = 3930 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t limit = 16 * n + 64;
    for (;;) {
        uint8_t *rep = (uint8_t *)calloc((size_t)limit + 1, 1);
        if (!rep) return (seq_t)0;
        for (idx_t i = 2; ; ++i) {
            bool any = false;
            for (idx_t j = 2; ; ++j) {
                idx_t sub;
                if ((i & 1) == 0)
                    sub = (j & 1) ? i : 0;
                else
                    sub = (j & 1) ? (i + j - 1) : j;
                idx_t v = (3 * i * j - sub) / 2;
                if (v > limit) break;
                rep[v] = 1;
                if (j == 2) any = true;
            }
            if (!any) break;
        }
        idx_t count = 0;
        seq_t ans = 0;
        for (idx_t v = 2; v <= limit; ++v) {
            if (!rep[v]) {
                ++count;
                if (count == n) { ans = (seq_t)v; break; }
            }
        }
        free(rep);
        if (count >= n) return ans;
        limit <<= 1;
    }
}
