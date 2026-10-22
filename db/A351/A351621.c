/*
 * A351621  a(1) = 1; a(n) = 1 + a(n-1) + Sum_{k=2..n} a(floor(n/k)).
 * https://oeis.org/A351621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = 1 + a(n-1) + Sum_{k=2..n} a(floor(n/k))
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;   /* one‑based cache: cache[i] = a(i) */
static idx_t cap = 0;         /* current capacity of cache (number of allocated slots) */

/* Ensure that indices 0..n are valid in cache */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;
    if (n < cap) return;

    idx_t newcap = n + 1;
    seq_t *tmp = (seq_t *)realloc((void *)cache, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);

    for (idx_t i = cap; i < newcap; ++i) tmp[i] = 0;
    cache = tmp;
    cap   = newcap;
}

/* OEIS A351621 – a(n) = 1 + a(n-1) + sum_{k=2..n} a(floor(n/k)) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    ensure_cache(n);

    /* initialise a(1) if not yet done */
    if (cache[1] == 0) cache[1] = (seq_t)1;

    /* compute all needed terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        seq_t sum = cache[i-1] + (seq_t)1;
        idx_t k = 2;
        while (k <= i) {
            idx_t v = i / k;            /* floor(i/k)  */
            idx_t j = i / v;            /* last k with same v */
            if (j > i) j = i;

            seq_t cnt = (seq_t)(j - k + 1);   /* number of k with floor(i/k)=v */
            sum += cache[v] * cnt;

            k = j + 1;
        }
        cache[i] = sum;
    }

    return cache[n];
}
