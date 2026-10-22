/*
 * A396926  The smallest k for which the square of at least one integer among
 *          {n, n+1, ..., n+k} divides the sum of the remaining integers.
 * https://oeis.org/A396926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A396926:  a(n) = the smallest k ≥ 1 such that
 *  ∃ m in {n, …, n+k} with m² | (sum_{i=n}^{n+k, i≠m} i)
 *
 *  The implementation uses a simple search for each n, memoising
 *  already computed values.  Divisibility by m² is tested via the
 *  equivalent conditions m | T and (T/m) | m, avoiding the need for
 *  a potentially overflowing square.  All arithmetic is performed
 *  with seq_t, which can be an 8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned
 *  integer or an arbitrary‑precision integer class, as required by
 *  the build system.  The function is O(n·k²) with k the answer for
 *  that n, which is fast enough for n up to about 10 000.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache holds a(1) .. a(cache_max).  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;

    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* Grow the cache if necessary. */
    if (n > cache_max) {
        size_t new_size = (size_t)(n + 1);          /* indices 0 .. n */
        seq_t *new_cache =
            (seq_t *)malloc(new_size * sizeof(seq_t));  /* C++ friendly cast */
        if (!new_cache) return (seq_t)0;           /* allocation failed */

        /* Copy old values. */
        for (idx_t i = 1; i <= cache_max; ++i) {
            new_cache[i] = cache[i];
        }

        /* Compute missing entries from cache_max+1 up to n. */
        for (idx_t cur = cache_max + 1; cur <= n; ++cur) {
            idx_t k = 1;                           /* k ≥ 1 */

            while (true) {
                /* Sum S = sum_{i=cur}^{cur+k} i = (k+1)*(2*cur + k)/2  */
                seq_t two_cur = (seq_t)(2 * cur);
                seq_t len     = (seq_t)(k + 1);
                seq_t sum     = (len * (two_cur + (seq_t)k)) / 2;

                bool found = false;

                for (idx_t m = cur; m <= cur + k; ++m) {
                    seq_t mm = (seq_t)m;
                    seq_t T  = sum - mm;          /* sum of others */

                    /* Check divisibility: mm | T and (T/mm) | mm */
                    seq_t rem = T % mm;
                    if (rem == 0) {
                        seq_t q = T / mm;
                        if (q % mm == 0) {
                            found = true;
                            break;
                        }
                    }
                }

                if (found) {
                    new_cache[cur] = k;
                    break;
                }
                ++k;
            }
        }

        /* Replace old cache. */
        free(cache);
        cache = new_cache;
        cache_max = n;
    }

    return cache[n];
}
