/*
 * A370610  Squares such that any two consecutive decimal digits differ by 1
 *          after arranging the digits in decreasing order.
 * https://oeis.org/A370610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th perfect square whose decimal digits, upon being
 *            sorted in decreasing order, form a consecutive string (each
 *            adjacent pair of digits differs by 1). The sequence is finite
 *            (currently 160 terms). The implementation searches all squares
 *            k² with k ≤ 99999, because a square with more th...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 115, u64 n <= 160,
 *            u128 n <= 160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Cache for memoisation of sequence terms. */
static seq_t *cache     = NULL; /* array of found squares */
static size_t cache_len = 0;    /* number of terms stored   */
static size_t cache_cap = 0;    /* allocated capacity      */
static idx_t  next_k    = 0;    /* next integer to test    */

/* Append a new term to the cache, expanding it if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term (1‑based).  If n exceeds the length of
   the (finite) sequence, 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n = 0 */

    /* Generate sufficient terms until we have at least n of them
       or we have exhausted the possible squares (k = 0 … 99999). */
    while (cache_len < (size_t)n && next_k <= 99999) {
        seq_t k   = (seq_t)next_k;
        seq_t sq  = k * k;

        bool seen[10] = { false };
        int  cnt      = 0;
        int  min_d    = 10;
        int  max_d    = -1;
        seq_t tmp     = sq;

        /* Extract digits of the square. */
        if (tmp == 0) {               /* special case for 0² */
            seen[0] = true;
            cnt = 1;
            min_d = max_d = 0;
        } else {
            while (tmp) {
                unsigned d = (unsigned)(tmp % 10);
                if (seen[d]) {        /* duplicate digit -> impossible */
                    cnt = -1;         /* flag as invalid */
                    break;
                }
                seen[d] = true;
                ++cnt;
                if ((int)d < min_d) min_d = d;
                if ((int)d > max_d) max_d = d;
                tmp /= 10;
            }
        }

        /* Check that digits are consecutive without gaps. */
        if (cnt > 0 && (max_d - min_d + 1) == cnt) {
            append_cache(sq);
        }

        ++next_k;                     /* advance to next k */
    }

    return (cache_len >= (size_t)n) ? cache[n - 1] : (seq_t)0;
}
