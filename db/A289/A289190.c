/*
 * A289190  Numbers k such that k^2 with last digit deleted is a prime.
 * https://oeis.org/A289190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 50, u64 n <= 545, u128 n <= 545,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A289190: k such that floor(k^2/10) is prime
   (i.e., delete the last digit of k^2 to obtain a prime).  This
   function builds the sequence incrementally, caching results so
   that subsequent calls are O(1) after the needed terms have been
   generated.  Prime testing uses the provided is_prime_big routine. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;       /* array of found terms */
static size_t cache_len = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* current capacity of cache */
static idx_t next_k = 1;          /* next integer k to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not part of the sequence, but safe */

    /* Generate terms until we have at least n terms */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_k;                 /* current k */
        seq_t sq = k * k;                        /* k^2 */
        seq_t candidate = sq / 10;               /* floor(k^2/10) */

        if (is_prime_big(candidate))
        {
            /* Store k as the next term of the sequence */
            append_candidate(&cache, &cache_len, &cache_cap, k);
        }
        next_k++;
    }

    /* Return the required term (0‑based indexing in cache) */
    return cache[n - 1];
}
