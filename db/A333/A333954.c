/*
 * A333954  Numbers k such that A330575(k) = A330575(k+1).
 * https://oeis.org/A333954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search -- sieve A330575(m) = m + Sum_{d|m, d<m} A330575(d) for
 *            all m up to a growing limit (native 64-bit values), and list the
 *            k with A330575(k) = A330575(k+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static idx_t *A333954_terms = NULL;
static size_t A333954_cnt = 0;
static size_t A333954_cap = 0;
static idx_t A333954_limit = 0;

static int A333954_scan(idx_t N)
{
    uint64_t *f = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    if (!f) return 0;
    for (idx_t m = 0; m <= N; ++m) f[m] = (uint64_t)m;
    for (idx_t d = 1; d <= N / 2; ++d)
    {
        uint64_t v = f[d];
        for (idx_t m = 2 * d; m <= N; m += d) f[m] += v;
    }
    A333954_cnt = 0;
    for (idx_t k = 1; k < N; ++k)
    {
        if (f[k] == f[k + 1])
        {
            if (A333954_cnt == A333954_cap)
            {
                size_t nc = A333954_cap ? A333954_cap * 2 : 64;
                idx_t *nt = (idx_t *)realloc(A333954_terms, nc * sizeof(idx_t));
                if (!nt) { free(f); return 0; }
                A333954_terms = nt;
                A333954_cap = nc;
            }
            A333954_terms[A333954_cnt++] = k;
        }
    }
    A333954_limit = N;
    free(f);
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A333954_cnt < (size_t)n)
    {
        idx_t nl = A333954_limit ? A333954_limit * 2 : ((idx_t)1 << 20);
        if (!A333954_scan(nl)) return 0; /* allocation failure */
    }
    return (seq_t)A333954_terms[n - 1];
}
