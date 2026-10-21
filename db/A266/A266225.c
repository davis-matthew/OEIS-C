/*
 * A266225  Least x>1 such that prime(n)*x+x-1 is a prime, or -1 if no such x
 *          exists.
 * https://oeis.org/A266225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1881, u32 n <= 225, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest x > 1 such that prime(n)*x + x - 1 is prime */
/* This function is memoised and works for any build of seq_t. */

#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* cache[i] stores a(i+1) */
static idx_t cachelen = 0;    /* number of entries currently cached */

/* Return a(n) for n >= 1.  If n == 0 the function returns 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined position */

    /* Resize cache if necessary */
    if (n > cachelen)
    {
        seq_t *newcache = seq_realloc(cache, (size_t)cachelen, (size_t)n);
        if (!newcache)
            return (seq_t)-1; /* allocation failure */
        cache = newcache;
        for (idx_t i = cachelen; i < n; ++i)
        {
            uint32_t p = get_prime((idx_t)(i + 1)); /* prime(n) ; get_prime is 1‑based */
            idx_t x = 2;
            while (1)
            {
                seq_t cand = (seq_t)x * (seq_t)(p + 1) - (seq_t)1;
                if (is_prime_big(cand))
                {
                    cache[i] = (seq_t)x;
                    break;
                }
                ++x;
            }
        }
        cachelen = n;
    }
    return cache[n - 1];
}
