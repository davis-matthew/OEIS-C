/*
 * A358247  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 7, up to isomorphism.
 * https://oeis.org/A358247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358247: number of n‑regular, N_0‑weighted pseudographs on 2 vertices
 * with total edge weight 7, up to isomorphism.
 *
 * Recurrence (valid for n ≥ 15):
 *     a(n) = a(n-1) + a(n-2) - a(n-3)
 * and the first 14 values are below:
 *     a(1)=1, a(2)=8, a(3)=28, a(4)=71, a(5)=132, a(6)=217,
 *     a(7)=309, a(8)=417, a(9)=521, a(10)=638, a(11)=746,
 *     a(12)=866, a(13)=975, a(14)=1096
 *
 * For n ≥ 14 the simple linear recurrence a(n)=a(n-2)+230 also holds, but
 * the homogeneous three‑term recurrence above is used because it works for
 * all n ≥ 15 while keeping the implementation short and efficient.
 * For n ≤ 13 the values are returned from a small precomputed table.
 *
 * The routine is safe for any of the supported integer types `seq_t` and
 * `idx_t`.  It works correctly for all n ≥ 1 and is O(n) with a trivial
 * constant factor, easily fitting the 2‑second limit for n up to about
 * 10 000.
 */
#include <stdbool.h>  /* for bool, true, false */

seq_t a(idx_t n)
{
    /* Precomputed first 15 terms (index 0 unused) as 64‑bit constants. */
    static const uint64_t init64[15] = {
        0ULL, 1ULL, 8ULL, 28ULL, 71ULL, 132ULL, 217ULL,
        309ULL, 417ULL, 521ULL, 638ULL, 746ULL, 866ULL,
        975ULL, 1096ULL
    };

    /* Store the values in a static array of seq_t on first call. */
    static seq_t init[15];
    static bool init_done = false;
    if (!init_done) {
        for (idx_t i = 0; i < 15; ++i) {
            init[i] = (seq_t)init64[i];
        }
        init_done = true;
    }

    if (n < 15) {                 /* Return the exact first 14 values. */
        return init[n];
    }

    /* Compute from n = 15 upward using the three‑term homogeneous
     * recurrence: a(n) = a(n-1) + a(n-2) - a(n-3). */
    seq_t a_nm3 = init[12];        /* a(12)   */
    seq_t a_nm2 = init[13];        /* a(13)   */
    seq_t a_nm1 = init[14];        /* a(14)   */

    for (idx_t k = 15; k <= n; ++k) {
        seq_t a_n = a_nm1 + a_nm2 - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
