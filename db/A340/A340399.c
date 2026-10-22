/*
 * A340399  a(n) is the least Fibonacci number >= 2^n.
 * https://oeis.org/A340399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least Fibonacci number >= 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2, seq_free, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_free.h"

static seq_t *fib_cache = NULL;  /* array of Fibonacci numbers */
static size_t fib_len  = 0;      /* number of elements stored (next free index) */
static size_t fib_cap  = 0;      /* allocated capacity */

/* Ensure fib_cache contains Fibonacci numbers up to index `idx` (inclusive) */
static void fib_ensure(idx_t idx)
{
    size_t needed = (size_t)idx + 1;
    if (needed <= fib_len) return;

    /* Grow capacity if necessary */
    if (fib_cap < needed) {
        size_t new_cap = fib_cap ? fib_cap : 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = seq_realloc(fib_cache, fib_cap, new_cap);
        fib_cache = tmp;
        fib_cap   = new_cap;
    }

    /* Initialise missing elements */
    if (fib_len == 0) { fib_cache[0] = 0; fib_len = 1; }
    if (fib_len == 1) { fib_cache[1] = 1; fib_len = 2; }

    for (size_t i = fib_len; i < needed; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = needed;
}

/* Return the least Fibonacci number >= 2^n */
seq_t a(idx_t n)
{
    /* Compute target = 2^n */
    seq_t target = pow2(n);

    /* Freely ready first two Fibonacci numbers */
    if (fib_len < 2) fib_ensure(1);

    /* Grow until the last Fibonacci number is >= target */
    while (fib_cache[fib_len-1] < target) {
        fib_ensure((idx_t)fib_len);
    }

    /* Binary search for the smallest index with fib_cache[idx] >= target */
    size_t lo = 0, hi = fib_len - 1;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (fib_cache[mid] >= target) hi = mid;
        else                          lo = mid + 1;
    }
    return fib_cache[lo];
}
