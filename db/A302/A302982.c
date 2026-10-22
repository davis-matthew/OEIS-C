/*
 * A302982  Number of ways to write n as x^2 + 5*y^2 + 2^z + 3*2^w with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A302982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y,z,w) ∈ ℕ⁴ : * n = x² + 5·y² + 2ᶻ + 3·2ʷ }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by exhaustive enumeration up to the
 * requested index.  Counts are held in a static uint64_t cache; the
 * result is cast to seq_t on return, which works for both unsigned
 * integer builds and for the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *cache = NULL;      /* array of counts, index 0 unused */
static idx_t cache_max = 0;         /* largest n for which cache is valid */

/* Compute the number of solutions for a given n.  The function is
 * memoised; a new cache is built if the requested n exceeds the
 * previous maximum. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 – return 0 */
        return 0;

    /* rebuild cache if necessary */
    if (n > cache_max) {
        idx_t new_max = n;
        /* allocate new cache (index 0 left unused) */
        uint64_t *new_cache = (uint64_t*)calloc((size_t)new_max + 1, sizeof(uint64_t));
        if (!new_cache) abort();   /* memory exhaustion – stop */

        /* limits for the loops – computed safely for 64‑bit unsigned */
        idx_t max_x = 0;
        while ((max_x + 1) * (max_x + 1) <= new_max) ++max_x;

        idx_t max_y = 0;
        while (5ULL * (max_y + 1) * (max_y + 1) <= new_max) ++max_y;

        idx_t max_z = 0;
        /* 2^z <= new_max  */
        while ((max_z + 1) < (sizeof(idx_t) * 8) &&
               (((idx_t)1) << (max_z + 1)) <= new_max)
            ++max_z;

        /* 3·2^w <= new_max  */
        idx_t max_w = 0;
        while ((max_w + 1) < (sizeof(idx_t) * 8) &&
               (3ULL * (((idx_t)1) << (max_w + 1))) <= new_max)
            ++max_w;

        /* enumerate all quadruples (x,y,z,w) */
        for (idx_t x = 0; x <= max_x; ++x) {
            idx_t x2 = x * x;
            for (idx_t y = 0; y <= max_y; ++y) {
                idx_t term_y = 5ULL * y * y;
                if (x2 + term_y > new_max) break;  /* further y will only increase */

                for (idx_t z = 0; z <= max_z; ++z) {
                    idx_t pow2z = ((idx_t)1) << z;
                    idx_t base = x2 + term_y + pow2z;
                    if (base > new_max) break;   /* further z will only increase */

                    for (idx_t w = 0; w <= max_w; ++w) {
                        idx_t pow2w = ((idx_t)1) << w;
                        idx_t val = base + 3ULL * pow2w;
                        if (val > new_max) break;   /* further w will only increase */
                        ++new_cache[val];
                    }
                }
            }
        }

        /* replace old cache */
        free(cache);
        cache = new_cache;
        cache_max = new_max;
    }

    /* return the cached count, cast to seq_t */
    return (seq_t)cache[n];
}
