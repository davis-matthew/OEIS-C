/*
 * A355809  a(n) is the number at the apex of a triangle whose base contains
 *          the distinct powers of 2 summing to n (in ascending order), and
 *          each number in a higher row is the sum of the two numbers directly
 *          below it; a(0) = 0.
 * https://oeis.org/A355809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the apex of a triangle whose base contains the distinct
 *            * powers of 2 summing to n. For n>0, if the set bits of n are *
 *            2^p0 < 2^p1 < … < 2^p(k-1) (k=popcount(n)) * then * * a(n) =
 *            Σ_{i=0}^{k-1} 2^pi * C(k-1,i) * * where C is the binomial
 *            coefficient. The recurrence * a(2*n) = 2*a(n) foll...
 * Range:     u8 n <= 94, u16 n <= 3838, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 64‑bit index type supplied by the build. */
extern seq_t a(idx_t n);

/* Build a Pascal triangle up to n=63; all values fit in seq_t. */
static seq_t binom_tbl[64][64];
static int binom_inited = 0;
static void init_binom(void)
{
    if (binom_inited) return;
    for (int n = 0; n < 64; ++n) {
        binom_tbl[n][0] = 1;
        binom_tbl[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            binom_tbl[n][k] = binom_tbl[n-1][k-1] + binom_tbl[n-1][k];
        }
    }
    binom_inited = 1;
}

/* Main sequence function.  Uses only integer arithmetic on seq_t. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    init_binom();

    /* popcount of n (popcount handles 64‑bit values). */
    unsigned int k = __builtin_popcountll((uint64_t)n);
    seq_t result = 0;
    unsigned int bit_index = 0;

    /* Iterate over bits in ascending order. */
    for (uint64_t pos = 0; pos < 64; ++pos) {
        if (n & ((uint64_t)1 << pos)) {
            seq_t weight = binom_tbl[k-1][bit_index];
            seq_t val    = (seq_t)((uint64_t)1 << pos);
            result += weight * val;
            ++bit_index;
        }
    }
    return result;
}
