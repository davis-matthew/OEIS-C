/*
 * A262159  a(1) = 1, for n > 1 the least composite number k > a(n-1) such
 *          that a(n-1) + k is also a composite number.
 * https://oeis.org/A262159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n>1, a(n) = the smallest composite k > a(n-1)
   such that a(n-1)+k is also composite. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* 0‑unused, cache[1] = a(1) */
static idx_t  max_computed = 0;

static void
ensure_cache(idx_t n)
{
    if (max_computed >= n)
        return;

    size_t new_size = (size_t)n + 1;          /* indices 0 … n, 0 unused */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                        /* out‑of‑memory */
    cache = new_cache;
    cache[0] = 0;                          /* unused entry */

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {                      /* a(1) = 1 */
            cache[1] = 1;
            continue;
        }

        seq_t prev = cache[i - 1];
        seq_t k = prev + (seq_t)1;           /* start testing after prev */

        for (;;) {
            if (k > (seq_t)1 && !is_prime_big(k)) {    /* k composite */
                seq_t sum = prev + k;
                if (sum > (seq_t)1 && !is_prime_big(sum)) {
                    cache[i] = k;          /* found a(n) */
                    break;
                }
            }
            ++k;                           /* next candidate */
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* guard against underflow, offset 1 */
        return 0;
    ensure_cache(n);
    return cache[n];
}
