/*
 * A345348  Triangular numbers that in base 2 have the same number of 0's and
 *          1's.
 * https://oeis.org/A345348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 4133, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A345348: Triangular numbers that in base 2 have the same number
   of 0’s and 1’s.  The n‑th such number is obtained by scanning
   successive triangular numbers m(m+1)/2 and keeping those whose
   binary expansion has an equal count of 0‑bits and 1‑bits.
   The implementation uses a memoised cache so that calls need
   not come in increasing order.  All arithmetic is performed
   with the build’s seq_t type, ensuring correctness for
   8, 16, 32, 64, 128‑bit unsigned integers and for arbitrary‑precision
   builds. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of found triangular numbers */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated cache capacity   */
    static size_t len   = 0;   /* number of terms cached     */
    static idx_t  last_m = 0;  /* last m processed           */

    if (n == 0) return 0;  /* meaning for indices < 1 is undefined */

    /* Grow the cache to hold at least n terms */
    if (len < (size_t)n) {
        /* Ensure capacity for storing up to n terms.  Use exponential
           growth to keep realloc calls O(log n). */
        if (cap < n) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – abort with zero */
                return 0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Scan triangular numbers until enough are found */
        while (len < (size_t)n) {
            /* Generate next triangular index m */
            ++last_m;

            /* T = m*(m+1)/2  (all in seq_t) */
            seq_t m_seq = (seq_t)last_m;
            seq_t t = m_seq * (m_seq + 1) / 2;

            /* Count bits and ones in t */
            seq_t tmp   = t;
            unsigned bits = 0;
            unsigned ones = 0;
            while (tmp) {
                if (tmp & 1) ++ones;
                ++bits;
                tmp >>= 1;
            }
            unsigned zeros = bits - ones;

            if (ones == zeros) {
                cache[len++] = t;
            }
        }
    }

    return cache[n - 1];
}
