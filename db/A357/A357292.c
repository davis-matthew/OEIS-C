/*
 * A357292  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least two elements of S) = difference
 *          between greatest two elements of S.
 * https://oeis.org/A357292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) - 2*a(n-4) - a(n-5) +
 *            2*a(n-6), a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* storage for computed values */
static idx_t  cache_cap = 0;       /* allocated capacity */
static idx_t  computed_len = 0;    /* highest index with a computed value */

static void ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;                    /* already big enough */

    idx_t newcap = n + 1;
    if (newcap < 8)                /* at least room for the base cases */
        newcap = 8;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)                     /* allocation failure */
        abort();

    cache = tmp;
    cache_cap = newcap;

    /* initialize the first six values on the very first allocation */
    if (computed_len == 0) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 1;
        computed_len = 5;          /* we now have values 0..5 ready */
    }
}

seq_t a(idx_t n)
{
    if (n <= 5) {                 /* base cases are already known */
        ensure_capacity(n);
        return cache[n];
    }

    /* grow the storage if necessary */
    ensure_capacity(n);

    /* compute missing values up to n */
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1]
                  + cache[i - 2]
                  - cache[i - 3]
                  - (seq_t)2 * cache[i - 4]
                  - cache[i - 5]
                  + (seq_t)2 * cache[i - 6];
        cache[i] = val;
    }

    computed_len = n;              /* we now have values up to n */
    return cache[n];
}
