/*
 * A249516  Numbers k for which the digital product A007954(k) contains the
 *          same distinct digits as the number k.
 * https://oeis.org/A249516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249516: a(n) is the nth integer k (starting with k = 0) such that
   the set of distinct decimal digits of k equals the set of distinct digits
   of the digit product of k (A007954(k)).  The implementation searches
   sequentially, memoising found terms in a static array. */
/* The function works for integer seq_t types (8/16/32/64/128-bit) and for
   arbitrary-precision seq_t classes compiled as C++17.  It does not use
   dynamic allocation to remain valid in both C and C++. */

#include <stddef.h>
#include <stdbool.h>

/* Utility to obtain the product of the decimal digits of an integer. */
#include "../utils/digit_product.h"

#define CACHE_CAP 20000      /* enough for n up to 10000th term */

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t last_k = 0;
static bool cache_inited = false;

/* Initialise the cache with the first term 0. */
static void init_cache(void)
{
    if (cache_inited) return;
    cache_inited = true;
    cache_len = 1;
    cache[0] = (seq_t)0;   /* a(1) = 0 */
    last_k  = 0;             /* next search starts from 1 */
}

/* Test whether integer k satisfies the property. */
static bool has_property(idx_t k)
{
    if (k == 0) return true;   /* 0 is always accepted */

    /* Mask of distinct digits of k.  0-disallowed for k > 0. */
    unsigned mask_k = 0;
    idx_t tmp = k;
    while (tmp > 0) {
        unsigned d = tmp % 10;
        if (d == 0) return false;          /* zero digit disqualifies */
        mask_k |= 1U << d;
        tmp /= 10;
    }

    /* Digit product of k. */
    seq_t prod = digit_product((seq_t)k);

    /* Mask of distinct digits of the product. */
    unsigned mask_p = 0;
    seq_t qp = prod;
    while (qp > 0) {
        unsigned d = (unsigned)(qp % 10);
        mask_p |= 1U << d;
        qp /= 10;
    }

    return mask_k == mask_p;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_n_terms(idx_t n)
{
    /* n is 1‑based; cache_len counts elements already found. */
    while (cache_len < (size_t)n && cache_len < (size_t)CACHE_CAP) {
        ++last_k;                       /* try next integer */
        if (has_property(last_k)) {
            cache[cache_len++] = (seq_t)last_k;
        }
    }
}

/* Public sequence function.  a(1) corresponds to the first term (0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; guard against accidental zero */
    init_cache();
    ensure_n_terms(n);
    return cache[n-1];
}
