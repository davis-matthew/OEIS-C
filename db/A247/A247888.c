/*
 * A247888  Zeroless numbers n such that n + A007954(n) contains the same
 *          digits as n.
 * https://oeis.org/A247888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: zeroless n such that the set of digits of n is the same as the
   set of digits of n + product_of_digits(n) (OEIS A247888). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether a candidate satisfies the defining property. */
static bool is_good(idx_t cand)
{
    seq_t n = (seq_t)cand;
    unsigned int mask_n = 0;

    /* first pass: build mask of digits of n, check for zeros. */
    seq_t tmp = n;
    if (tmp == 0)
        return false;
    while (tmp > 0)
    {
        unsigned int d = (unsigned int)(tmp % (seq_t)10);
        if (d == 0)
            return false;          /* zeroless requirement */
        mask_n |= 1u << d;
        tmp /= (seq_t)10;
    }

    /* product of digits of n */
    seq_t prod = (seq_t)1;
    tmp = n;
    while (tmp > 0)
    {
        unsigned int d = (unsigned int)(tmp % (seq_t)10);
        prod *= (seq_t)d;
        tmp /= (seq_t)10;
    }

    /* n + product */
    seq_t sum = n + prod;

    /* mask of digits of sum */
    unsigned int mask_s = 0;
    tmp = sum;
    while (tmp > 0)
    {
        unsigned int d = (unsigned int)(tmp % (seq_t)10);
        mask_s |= 1u << d;
        tmp /= (seq_t)10;
    }

    return mask_n == mask_s;
}

/* Cache of discovered terms.  The buffer is large enough for thousands
   of terms; grow if needed. */
#define CACHE_CAP 200000u

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static idx_t scanned = 0;   /* last number that was examined */

/* Return the n-th term (n ≥ 1) of Sequence A247888. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for index 0 */

    if ((idx_t)cache_len >= n)
        return cache[(size_t)(n - 1)];

    for (idx_t cur = scanned + 1; ; ++cur)
    {
        if (is_good(cur))
        {
            if (cache_len < CACHE_CAP)
                cache[cache_len++] = (seq_t)cur;
            scanned = cur;

            if ((idx_t)cache_len == n)
                return cache[(size_t)(n - 1)];
        }
    }
}
