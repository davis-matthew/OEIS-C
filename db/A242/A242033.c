/*
 * A242033  a(n) = lpf(A245024(n)-1), where lpf = least prime factor
 *          (A020639).
 * https://oeis.org/A242033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf(A245024(n)-1); A245024 = even m with lpf(m-1) <
 *            lpf(m-3). Computed by scanning even m with trial division,
 *            memoised
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9009 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t A242033_lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

static idx_t *A242033_cache = NULL;
static size_t A242033_cap = 0;
static idx_t  A242033_len = 0;
static idx_t  A242033_next = 10;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (A242033_len < n) {
        idx_t m = A242033_next;
        idx_t l1 = A242033_lpf(m - 1);
        idx_t l2 = A242033_lpf(m - 3);
        if (l1 < l2) {
            if ((size_t)A242033_len == A242033_cap) {
                size_t nc = A242033_cap ? A242033_cap * 2 : 64;
                idx_t *t = (idx_t *)realloc(A242033_cache, nc * sizeof(idx_t));
                if (!t) abort();
                A242033_cache = t;
                A242033_cap = nc;
            }
            A242033_cache[A242033_len++] = l1;
        }
        A242033_next += 2;
    }
    return (seq_t)A242033_cache[n - 1];
}
