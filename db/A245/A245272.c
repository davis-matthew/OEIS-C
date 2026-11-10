/*
 * A245272  Composite terms in sequence {A245024(n)-3}.
 * https://oeis.org/A245272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th composite m = A245024(k)-3, i.e. odd composite m
 *            with lpf(m+2) < lpf(m); found by search
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t A245272_lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

static idx_t *A245272_cache = NULL;
static size_t A245272_cap = 0;
static idx_t A245272_len = 0;
static idx_t A245272_next = 9;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A245272_len < n) {
        idx_t m = A245272_next;
        A245272_next += 2;
        idx_t l1 = A245272_lpf(m);
        if (l1 == m) continue;              /* m must be composite */
        idx_t l2 = A245272_lpf(m + 2);
        if (l2 < l1) {
            if ((size_t)A245272_len == A245272_cap) {
                size_t nc = A245272_cap ? A245272_cap * 2 : 64;
                idx_t *t = (idx_t *)realloc(A245272_cache, nc * sizeof(idx_t));
                if (!t) abort();
                A245272_cache = t;
                A245272_cap = nc;
            }
            A245272_cache[A245272_len++] = m;
        }
    }
    return (seq_t)A245272_cache[n - 1];
}
