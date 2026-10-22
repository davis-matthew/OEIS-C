/*
 * A308055  a(1)=2; for n > 1, a(n) is the square root of the smallest square
 *          with a(n-1) as a prefix in base 10.
 * https://oeis.org/A308055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(1) = 2; * For n > 1, a(n) is the square root of the smallest
 *            square * whose decimal representation starts with a(n-1)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   ensure_cache, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence on demand and caches the
 * results.  The file floor_sqrt.h declares floor_sqrt which returns
 * the integer square‑root (floor).  The helper ensure_cache guarantees
 * that a storage buffer is large enough. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_cache.h"

static seq_t *seq_cache = NULL;          /* array of values a(1)…a(n)            */
static size_t cache_cap = 0;             /* current allocated capacity          */
static size_t cache_size = 0;            /* number of terms already computed    */

/* Compute the next term from the previous term.
 * Finds the smallest integer m such that m² has the previous term as
 * leading decimal digits and returns m. */
static inline seq_t next_value(seq_t prev)
{
    seq_t power10 = 1;  /* 10^k for increasing k */
    while (1) {
        seq_t lower = prev * power10;
        seq_t upper = (prev + 1) * power10;
        upper -= 1;                     /* (prev+1)*10^k - 1 */

        /* lower <= m² <= upper is required */
        seq_t m_min = floor_sqrt(lower);
        if (m_min * m_min < lower) ++m_min;   /* ceil sqrt of lower */
        seq_t m_max = floor_sqrt(upper);

        if (m_min <= m_max)
            return m_min;                /* first square with the prefix */

        /* try the next possible length of the square */
        power10 *= 10;
    }
}

/* Return the n‑th term of the sequence, with 1‑based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against underflow */

    /* Allocate space for up to n terms if needed */
    if ((size_t)n > cache_cap)
        ensure_cache(n, &seq_cache, &cache_cap);

    /* Compute missing terms up to n */
    if (cache_size == 0) {                    /* first term */
        seq_cache[0] = (seq_t)2;
        cache_size = 1;
    }
    while (cache_size < (size_t)n) {
        seq_t prev = seq_cache[cache_size - 1];
        seq_t curr = next_value(prev);
        seq_cache[cache_size] = curr;
        ++cache_size;
    }

    /* Return the requested term */
    return seq_cache[(size_t)n - 1];
}
