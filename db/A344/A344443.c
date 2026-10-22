/*
 * A344443  Completely additive with a(2)=5; for odd prime p, a(p) =
 *          ceiling((a(p-1) + a(p+1))/2).
 * https://oeis.org/A344443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Completely additive sequence A344443
   a(1) = 0, a(2) = 5
   For odd prime p: a(p) = ceil((a(p-1) + a(p+1))/2)
   For all m,n: a(m*n) = a(m) + a(n)
   (Computed by prime factorisation and the above recurrence) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache      = NULL;   /* cached values */
static bool *cache_known = NULL;   /* true if cache[n] is valid */
static idx_t cache_cap   = 0;      /* current capacity of cache arrays */

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = n + 1;

    seq_t *new_cache      = (seq_t*)realloc(cache,      new_cap * sizeof(seq_t));
    bool   *new_known     = (bool*)realloc(cache_known, new_cap * sizeof(bool));
    if (!new_cache || !new_known) abort();            /* allocation failure */

    /* initialise newly added entries as unknown */
    for (idx_t i = cache_cap; i < new_cap; ++i) new_known[i] = false;

    cache      = new_cache;
    cache_known = new_known;
    cache_cap   = new_cap;
}

/* Sum contributions of prime powers in n:  sum_{p^e|n} e * a(p) */
static seq_t factor_sum(idx_t n)
{
    seq_t res = 0;
    idx_t temp = n;

    while (temp > 1) {
        idx_t p;      /* prime factor */
        idx_t exp = 0;

        /* find smallest prime factor */
        if (temp % 2 == 0) {
            p = 2;
            while (temp % 2 == 0) {
                temp /= 2;
                ++exp;
            }
        } else {
            p = 0;
            for (idx_t d = 3; d * d <= temp; d += 2) {
                if (temp % d == 0) {
                    p = d;
                    break;
                }
            }
            if (p == 0) {              /* temp itself is prime */
                p = temp;
                exp = 1;
                temp = 1;
            } else {
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
            }
        }

        res += (seq_t)exp * a(p);
    }

    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;

    ensure_cache(n);
    if (cache_known[n]) return cache[n];

    seq_t val;
    if (n == 2) {
        val = 5;
    } else if (is_prime(n)) {
        seq_t left  = a(n - 1);
        seq_t right = a(n + 1);
        seq_t sum   = left + right;
        val = sum / 2;
        if (sum % 2 != 0) ++val;      /* ceil */
    } else {
        val = factor_sum(n);
    }

    cache[n]      = val;
    cache_known[n] = true;
    return val;
}
