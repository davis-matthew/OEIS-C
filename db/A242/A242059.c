/*
 * A242059  lpf_3(A242057(n)-1), where lpf_3(n) = lpf(n/3^t) (cf. A020639)
 *          such that 3^t (t>=0) is the maximal power of 3 which divides n.
 * https://oeis.org/A242059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf_3(A242057(n)-1); computed by a memoised incremental
 *            search over even m with lpf_3(m-1) < lpf_3(m-3), a(n) =
 *            lpf_3(m-1)
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9731 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A242059_lpf3(uint64_t x)
{
    if (x <= 1ULL) return 1ULL;
    while (x % 3ULL == 0ULL) x /= 3ULL;
    if (x == 1ULL) return 1ULL;
    if (x % 2ULL == 0ULL) return 2ULL;
    for (uint64_t i = 5ULL; i <= x / i; i += 2ULL)
        if (i % 3ULL != 0ULL && x % i == 0ULL) return i;
    return x;
}

static uint64_t *A242059_vals = NULL;
static size_t A242059_len = 0, A242059_cap = 0;
static uint64_t A242059_m = 4ULL;      /* next even candidate */
static uint64_t A242059_prev = 1ULL;   /* lpf_3(m-3) for candidate m */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A242059_len < (size_t)n)
    {
        uint64_t l1 = A242059_lpf3(A242059_m - 1ULL);
        if (l1 < A242059_prev)
        {
            if (A242059_len == A242059_cap)
            {
                size_t nc = A242059_cap ? A242059_cap * 2 : 256;
                A242059_vals = (uint64_t *)realloc(A242059_vals, nc * sizeof(uint64_t));
                A242059_cap = nc;
            }
            A242059_vals[A242059_len++] = l1;
        }
        A242059_prev = l1;
        A242059_m += 2ULL;
    }
    return (seq_t)A242059_vals[n - 1];
}
