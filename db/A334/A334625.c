/*
 * A334625  Maximal size of a subset T of S = {1,2,...,n} with a cyclic
 *          arrangement of T such that any three neighbors can be reordered in
 *          an arithmetic progression.
 * https://oeis.org/A334625
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1)+a(n-6)-a(n-7)  for n > 15
 * Initial values (for 3 ≤ n ≤ 15):
 *    a(3)=a(4)=a(5)=a(6)=a(7)=a(8)=3,
 *    a(9)=a(10)=a(11)=a(12)=9,
 *    a(13)=a(14)=12,
 *    a(15)=15.
 * For n < 3 the function returns 0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i) for i ≥ 0 */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static size_t cache_len = 0;  /* first unused index (i.e. cached values are 0..cache_len-1) */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* undefined below offset */

    /* ensure we have room for index n */
    size_t idx = (size_t)n;              /* n is u64, we only handle n that fit in size_t */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= idx) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing values up to n */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)0;
        } else if (i <= 8) {
            cache[i] = (seq_t)3;
        } else if (i <= 12) {
            cache[i] = (seq_t)9;
        } else if (i <= 14) {
            cache[i] = (seq_t)12;
        } else if (i == 15) {
            cache[i] = (seq_t)15;
        } else {
            /* recurrence: a(i) = a(i-1) + a(i-6) - a(i-7) */
            cache[i] = cache[i - 1] + cache[i - 6] - cache[i - 7];
        }
    }
    cache_len = idx + 1;                 /* now cache contains up to n */

    return cache[idx];
}
