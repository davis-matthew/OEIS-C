/*
 * A391577  Numbers k such that A048720(3,k) = 3*m for some m < k, where
 *          A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer k ≥ 1 for which - k has at least one
 *            pair of adjacent 1‑bits in binary, - (k XOR (k << 1)) is
 *            divisible by 3, - ((k XOR (k << 1)) / 3) < k. These conditions
 *            define OEIS A391577
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *cache   = NULL;   /* stored values of the sequence */
static idx_t  size    = 0;      /* number of elements in cache   */
static idx_t  capacity= 0;      /* allocated capacity            */
static idx_t  next_k  = 1;      /* smallest not yet tested k     */

/* Ensure the cache can hold at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (capacity < need) {
        idx_t new_cap = capacity ? capacity << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)realloc(cache,
                                 new_cap * sizeof(seq_t));
        capacity = new_cap;
    }
}

/* Test whether k satisfies the defining property. */
static bool qualifies(idx_t k)
{
    /* Adjacent 1‑bits check: k & (k>>1) != 0 */
    if ((k & (k >> 1)) == 0)
        return false;

    seq_t prod = (seq_t)k ^ ((seq_t)k << 1);   /* carryless product of 3 and k */
    if (prod % 3 != 0)
        return false;

    seq_t m = prod / 3;
    return m < k;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, but safe */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        while (!qualifies(next_k))
            ++next_k;                     /* look for the next valid k */

        grow_cache(size + 1);             /* ensure capacity */
        cache[size++] = (seq_t)next_k;    /* store the term */
        ++next_k;                         /* start searching after it */
    }
    return cache[n - 1];                  /* a(1) is at index 0 */
}
