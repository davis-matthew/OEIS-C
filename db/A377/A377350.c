/*
 * A377350  E.g.f. satisfies A(x) = 1 - log(1 - x*A(x)^3)/A(x)^3.
 * https://oeis.org/A377350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((3*n+1)/4)} (3*n-3*k)!/(3*n-4*k+1)! *
 *            |Stirling1(n,k)|
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A377350 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* maximum k needed */
    idx_t kmax = (3 * n + 1) / 4;

    /* allocate Stirling numbers array s[0..kmax] */
    seq_t *s = seq_alloc_new_array((size_t)(kmax + 1));
    for (size_t i = 0; i <= (size_t)kmax; ++i) s[i] = 0;
    s[0] = 1;                 /* s(0,0) = 1 */

    /* Compute unsigned Stirling numbers of the first kind for n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t maxk = i <= kmax ? i : kmax;
        for (idx_t k = maxk; k >= 1; --k) {
            seq_t tmp = mul_small(s[k], (unsigned int)(i - 1));
            s[k] = tmp + s[k - 1];
        }
        s[0] = 0;           /* s(n,0)=0 for n>0 */
    }

    /* Sum over k */
    seq_t result = 0;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t start = 3 * n - 4 * k + 2;   /* first factor in the ratio */
        idx_t end   = 3 * n - 3 * k;       /* last factor in the ratio */
        seq_t r = 1;
        if (start <= end) {
            for (idx_t f = start; f <= end; ++f) {
                r = mul_small(r, (unsigned int)f);
            }
        }
        seq_t term = seq_mul(s[k], r);
        result = result + term;
    }

    free_seq_array(s);
    return result;
}
