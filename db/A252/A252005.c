/*
 * A252005  a(1)=2; for n>1, a(n) is the least prime > a(n-1) such that sum of
 *          digits of a(n-1) and a(n) is prime.
 * https://oeis.org/A252005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 846, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; for n>1, a(n) is the least prime > a(n-1) such that
   digit_sum(a(n-1))+digit_sum(a(n)) is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"   /* seq_t digit_sum(seq_t v); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

/* Return the smallest prime strictly greater than start. */
static seq_t next_prime_after(seq_t start)
{
    seq_t cand = start + (seq_t)1;
    while (!is_prime_big(cand))
        ++cand;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined below offset; return 0 for safety */
        return 0;

    static seq_t *cache = NULL;   /* 0‑indexed array of computed terms */
    static idx_t cache_len = 0;   /* number of terms stored in cache */

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Expand cache to hold term n */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();        /* allocation failure */
    cache = new_cache;

    /* Compute missing terms sequentially */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)2;
            continue;
        }

        seq_t prev = cache[i - 2];   /* previous term (0‑based) */

        /* Find the least prime > prev that satisfies the digit‑sum condition */
        seq_t cand = next_prime_after(prev);
        while (true) {
            seq_t sum = digit_sum(prev) + digit_sum(cand);
            if (is_prime_big(sum))
                break;
            cand = next_prime_after(cand);
        }

        cache[i - 1] = cand;
    }

    cache_len = n;
    return cache[n - 1];
}
