/*
 * A277153  Lexicographically least sequence of nonnegative integers that
 *          avoids 9/7-powers.
 * https://oeis.org/A277153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically least sequence of nonnegative integers
 *            that contains no factor of exponent 9/7 (i.e. no block of length
 *            9k consisting of a 7k‑segment followed by its first 2k symbols)
 * Range:     u8 n <= 9912, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 256;
    while (newcap < needed) newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr) exit(EXIT_FAILURE);  /* abort on OOM */
    cache = newptr;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* generate sequence up to index n on demand */
    if (n >= cache_len) {
        ensure_capacity((size_t)n + 1);
        while (cache_len <= n) {
            /* find the smallest nonnegative integer that keeps the
               prefix free of 9/7‑powers after appending it.                */
            seq_t candidate = 0;
            for (;;) {
                bool bad = false;
                idx_t newLen = cache_len + 1;
                /* check all possible 9/7‑powers that end at newLen-1   */
                for (idx_t k = 1; 9 * k <= newLen; ++k) {
                    idx_t start = newLen - 9 * k;        /* start of power */
                    idx_t blockStart = start;           /* start of 7k block */
                    idx_t restStart  = blockStart + 7 * k; /* start of 2k tail */

                    /* compare the 2k tail with the first 2k symbols of the block */
                    idx_t i;
                    for (i = 0; i < 2 * k; ++i) {
                        seq_t valBlock = cache[blockStart + i];
                        seq_t valRest;
                        if (restStart + i == newLen - 1)
                            valRest = candidate;
                        else
                            valRest = cache[restStart + i];
                        if (valRest != valBlock) break;
                    }
                    if (i == 2 * k) {       /* all matched -> forbidden pattern */
                        bad = true;
                        break;
                    }
                }
                if (!bad) break;   /* candidate is acceptable */
                ++candidate;
            }
            cache[cache_len++] = candidate;
        }
    }
    return cache[(size_t)n];
}
