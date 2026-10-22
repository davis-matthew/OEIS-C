/*
 * A308469  a(1) = 1, a(2)=2, a(n) = a(n-1) + gcd(a(n-2), n-2).
 * https://oeis.org/A308469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n)=a(n-1)+gcd(a(n-2), n-2) for n>=3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* cache[1..cap] holds computed a(i)   */
static idx_t  cap   = 0;      /* current capacity of cache           */
static idx_t  high  = 0;      /* highest index that has been computed*/

/* make sure the cache is large enough to hold index n */
static void ensure_capacity(idx_t n)
{
    if (n <= cap) return;
    idx_t new_cap = cap ? cap * 2 : 2;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    ensure_capacity(n);

    /* initialise the first two terms on demand */
    if (high < 1) { cache[1] = 1; high = 1; }
    if (high < 2) { cache[2] = 2; high = 2; }

    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* compute up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        seq_t g = gcd(cache[i - 2], (seq_t)(i - 2));
        cache[i] = cache[i - 1] + g;
    }
    high = n;
    return cache[n];
}
