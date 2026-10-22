/*
 * A358553  Number of internal (non-leaf) nodes in the n-th standard ordered
 *          rooted tree.
 * https://oeis.org/A358553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (for the root) + Σ a(part) where the part‐composition
 *            of n−1 is taken in graded reverse‑lexicographic order
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the build supplies the types `idx_t` (unsigned 64‑bit) and `seq_t` (unsigned integer or big‑int) */
#include <stdint.h>
#include <stddef.h>

/* maximum n for which we keep a cache – 10000 is comfortably larger than the
   values used in the tests. */
#define MAX_CACHE 10001
static seq_t cache[MAX_CACHE];
static unsigned char cache_filled[MAX_CACHE] = {0};

/* floor_log2 for a 64‑bit unsigned integer.
   For GCC/Clang we use the builtin; otherwise a simple loop. */
static inline idx_t floor_log2_u64(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    /* v > 0 for all calls */
    return 63 - __builtin_clzll((unsigned long long)v);
#else
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
#endif
}

/* recursive computation of a(n) with memoisation */
static seq_t a_recursive(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n < MAX_CACHE && cache_filled[n]) return cache[n];

    idx_t r = n - 1;          /* rank of the composition                 */
    idx_t m = 0;              /* sum of the composition (number of parts) */
    if (r != 0) m = floor_log2_u64(r) + 1;   /* m = floor_log2(r)+1   */

    seq_t total = (seq_t)1;   /* root is internal (m>0 for n>1) */

    idx_t sum_m  = m;                    /* remaining sum to break  */
    idx_t rank_in = 0;                    /* rank inside current sum block */
    if (m > 0) rank_in = r - ((idx_t)1 << (m - 1));

    while (sum_m > 0) {
        idx_t first = sum_m;
        idx_t rem, count;
        for (;;) {
            rem = sum_m - first;
            if (rem == 0) {
                count = 1;
            } else {
                /* number of compositions of the remaining sum rem
                   equals 2^(rem-1).  If rem-1 >= 63 the value is
                   definitely larger than rank_in for all n we handle. */
                if (rem - 1 >= 63) {
                    count = rank_in + 1;   /* guarantees count > rank_in */
                } else {
                    count = ((idx_t)1 << (rem - 1));
                }
            }
            if (rank_in >= count) {
                rank_in -= count;
                first--;                   /* try next smaller first part */
            } else {
                break;
            }
        }
        total += a_recursive((idx_t)first);
        sum_m -= first;            /* consume the chosen part */
        /* rank_in is already correct for the remaining sum */
    }

    if (n < MAX_CACHE) {
        cache_filled[n] = 1;
        cache[n] = total;
    }
    return total;
}

seq_t a(idx_t n)
{
    return a_recursive(n);
}
