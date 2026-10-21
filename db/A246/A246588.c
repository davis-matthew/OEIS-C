/*
 * A246588  Run Length Transform of S(n) = wt(n) = 0,1,1,2,1,2,2,3,1,... (cf.
 *          A000120).
 * https://oeis.org/A246588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ wt(run length of each consecutive block of 1s in
 *            binary(n)) where wt(k) is the binary weight (popcount) of k. By
 *            convention a(0)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: popcount for idx_t (uint64_t) */
static inline seq_t popcount_idx(idx_t v)
{
#if defined(__GNUC__) || defined(__clang__)
    return (seq_t)__builtin_popcountll(v);
#else
    /* Portable implementation */
    seq_t c = 0;
    while (v) {
        c += (seq_t)(v & 1);
        v >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t x = n;

    while (x != 0) {
        /* Skip trailing zeros (bits before the next run of 1s) */
        while ((x & 1ULL) == 0ULL) {
            x >>= 1;
            if (x == 0) break;
        }
        if (x == 0) break;

        /* Count consecutive 1s to get the run length */
        idx_t runlen = 0;
        while ((x & 1ULL) == 1ULL) {
            runlen++;
            x >>= 1;
        }

        /* Multiply by wt(runlen) = popcount(runlen) */
        result *= popcount_idx(runlen);
    }
    return result;
}
