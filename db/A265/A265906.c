/*
 * A265906  a(n) = A153880(A265905(n)); also the first differences of A265905.
 * https://oeis.org/A265906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A153880(A265905(n)); A265905(1) = 1, A265905(n) =
 *            A265905(n-1) + A153880(A265905(n-1)); A153880(x) = Sum_{k>=1}
 *            d_k*(k+1)!, d_k = factorial-base digits of x
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 109 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A153880_a(seq_t x)
{
    seq_t result = 0;
    seq_t fac = 2;
    idx_t i = 1;
    while (x > 0)
    {
        seq_t d = x % (seq_t)(i + 1);
        result += d * fac;
        x = (x - d) / (seq_t)(i + 1);
        fac *= (seq_t)(i + 2);
        ++i;
    }
    return result;
}

static seq_t *a265906_cache = NULL;   /* a265906_cache[k] = A265905(k+1) */
static size_t a265906_len = 0;
static size_t a265906_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (a265906_len == 0) {
        a265906_cap = 64;
        a265906_cache = mem_new(seq_t, a265906_cap);
        a265906_cache[0] = (seq_t)1;
        a265906_len = 1;
    }
    if (a265906_cap < (size_t)n) {
        size_t nc = a265906_cap * 2;
        if (nc < (size_t)n) nc = (size_t)n;
        a265906_cache = mem_resize(seq_t, a265906_cache, nc);
        a265906_cap = nc;
    }
    while (a265906_len < (size_t)n) {
        seq_t prev = a265906_cache[a265906_len - 1];
        a265906_cache[a265906_len] = prev + A153880_a(prev);
        a265906_len++;
    }
    return A153880_a(a265906_cache[(size_t)(n - 1)]);
}
