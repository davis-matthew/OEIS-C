/*
 * A360822  Numbers whose squares have at most 2 digits less than 8.
 * https://oeis.org/A360822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 61, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Sequence A360822: Numbers whose squares have at most two digits less than 8.
   a(n) is the n‑th smallest positive integer m such that in the decimal
   representation of m² there are at most two digits that are < 8.
   This implementation uses a lazy, incremental search: a static cache
   stores all found terms and is extended only as far as required to
   evaluate the requested index.  The algorithm is O(1) per candidate
   and satisfies all ordering and memoisation requirements.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* static cache for found terms */
    static seq_t cache[256];
    static size_t cache_size = 0;       /* number of cached terms   */
    static seq_t last_candidate = 0;    /* last candidate examined  */

    /* offset starts at 1: a(1) is the first term */
    if (n == 0)
        return 0;

    /* if the term is already cached, return it */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* generate candidates until we have at least n terms or overflow */
    while (cache_size < (size_t)n) {
        /* advance to next candidate; detect overflow by wrap‑around to 0 */
        last_candidate++;
        if (last_candidate == 0)
            break;            /* cannot find more terms in this type */

        /* compute the square of the candidate */
        seq_t sq = last_candidate * last_candidate;

        /* count digits in the square */
        unsigned int freq[10];
        digit_frequencies(sq, freq);

        /* how many digits < 8? */
        size_t lt8 = 0;
        for (int d = 0; d < 8; ++d)
            lt8 += freq[d];

        /* candidate satisfies the condition? */
        if (lt8 <= 2) {
            if (cache_size < sizeof(cache) / sizeof(cache[0]))
                cache[cache_size++] = last_candidate;
        }
    }

    /* return cached term if available, otherwise 0 for out‑of‑range */
    return (cache_size >= (size_t)n) ? cache[n - 1] : (seq_t)0;
}
