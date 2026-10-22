/*
 * A358857  Least integer k in A031443 such that k*n is also in A031443, or -1
 *          if there is no such k.
 * https://oeis.org/A358857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = least k such that k is in A031443 (balanced binary) and
 *            k*n is also in A031443; a(n) = -1 if no such k exists
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 2045, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether x has an equal number of 0's and 1's in its binary representation. */
static inline bool is_balanced(seq_t x)
{
    if (x <= 1)
        return false;   /* 0 and 1 never have equal 0/1 counts */

    unsigned ones = 0;
    unsigned bitlen = 0;
    seq_t t = x;
    while (t > 0) {
        ones += (unsigned)(t & 1);
        t >>= 1;
        bitlen++;
    }
    return (ones << 1) == bitlen;   /* 2*ones == bitlen */
}

/* --------------------------------------------------------------------- */
/*  Simple dynamic cache for already computed values                       */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;   /* 0 indicates not yet computed */
static size_t cache_cap = 0;

static void maybe_expand_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap << 1 : 1024;
    while (newcap <= n)
        newcap <<= 1;

    seq_t *old = cache;
    seq_t *newp = (seq_t *)calloc(newcap, sizeof(seq_t));
    if (old) {
        memcpy(newp, old, cache_cap * sizeof(seq_t));
        free(old);
    }
    cache = newp;
    cache_cap = newcap;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; outside the sequence */

    maybe_expand_cache(n);
    seq_t *slot = &cache[n - 1];
    if (*slot != 0)
        return *slot;      /* already computed */

    /* Powers of two greater than 1 have no suitable k. */
    if (n > 1 && (n & (n - 1)) == 0) {
        *slot = (seq_t)(-1);
        return *slot;
    }

    seq_t ans = -1;
    seq_t idx = (seq_t)n;

    /* Search upward for the least k in A031443 with k*n also balanced. */
    for (seq_t k = 2; ; ++k) {
        if (!is_balanced(k))
            continue;

        seq_t prod = k * idx;  /* relies on seq_t multiplication; safe for arbitrary-precision or 128-bit */
        if (is_balanced(prod)) {
            ans = k;
            break;
        }
    }

    *slot = ans;
    return ans;
}
