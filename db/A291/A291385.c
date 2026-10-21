/*
 * A291385  a(n) = (1/4)*A073388(n+1).
 * https://oeis.org/A291385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*A073388(n+1). Recurrence: a(0)=1, a(1)=4, a(2)=14,
 *            a(3)=47, a(4)=152. For n >= 5: a(n) = 4*a(n-1) - 8*a(n-3) -
 *            4*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;                       /* already computed */
    idx_t new_len = n + 1;
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (new_cache == NULL)
        abort();                      /* allocation failure */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)      new_cache[0] = (seq_t)1;
        else if (i == 1) new_cache[1] = (seq_t)4;
        else if (i == 2) new_cache[2] = (seq_t)14;
        else if (i == 3) new_cache[3] = (seq_t)47;
        else if (i == 4) new_cache[4] = (seq_t)152;
        else {
            seq_t t1 = (seq_t)4 * new_cache[i-1];
            seq_t t2 = (seq_t)8 * new_cache[i-3];
            seq_t t3 = (seq_t)4 * new_cache[i-4];
            new_cache[i] = t1 - t2 - t3;      /* safe: t1 > t2+t3 for all n */
        }
    }
    cache_len = new_len;
    cache = new_cache;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
