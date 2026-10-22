/*
 * A396231  Sort digits in descending order then add to original number,
 *          repeat, starting with 1.
 * https://oeis.org/A396231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n+1) = a(n) + A004186(a(n)), a(1) = 1 * where A004186(x) is
 *            the number obtained by sorting the decimal * digits of x in
 *            descending order (OEIS A004186)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 32, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Reorder the decimal digits of v in descending order. */
static seq_t sort_desc(seq_t v)
{
    if (v == 0) return 0;
    unsigned char cnt[10] = {0};
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        ++cnt[d];
        v = v / 10;
    }
    seq_t res = 0;
    for (int d = 9; d >= 0; --d) {
        for (unsigned i = 0; i < cnt[d]; ++i) {
            res = res * 10 + (seq_t)d;
        }
    }
    return res;
}

/* Cache for already computed terms. */
static seq_t *cache = NULL;
static idx_t sz = 0;
static idx_t cap = 0;

/* Ensure the cache can hold at least `needed` entries. */
static void ensure_cap(idx_t needed)
{
    if (needed <= cap) return;
    idx_t newcap = cap ? cap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    seq_t *newarr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newarr) abort();   /* Allocation failure – abort. */
    cache = newarr;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; n=0 is not used. */
    if (sz == 0) {
        /* Initialize cache with a(1) = 1. */
        ensure_cap(1);
        cache[0] = (seq_t)1;
        sz = 1;
    }
    while (sz < n) {
        /* Grow cache if necessary before accessing cache[sz]. */
        ensure_cap(sz + 1);
        seq_t prev = cache[sz - 1];
        cache[sz] = prev + sort_desc(prev);
        ++sz;
    }
    return cache[n - 1];
}
