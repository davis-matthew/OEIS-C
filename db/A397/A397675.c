/*
 * A397675  Table of D(p, n) read by antidiagonals: Difference sequence of the
 *          generalized p-peg Tower of Hanoi (Frame-Stewart algorithm).
 * https://oeis.org/A397675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 2080,
 *            u128 n <= 8256, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397675 – table of D(p, n) read by antidiagonals.
   D(p, n) = 2^k where k is the smallest integer with
   binomial(k + p – 2, p – 2) ≥ n + 1.
   The enumeration orders the entries by S = p + n increasing,
   and for each S lists them in order of decreasing p
   (equivalently increasing disk index n). */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;          /* undefined for offset 0 */

    /* --- Find the diagonal sum S such that the first S diagonals
       contain at least n entries.  The number of entries up to
       diagonal S is f(S) = (S-2)(S-1)/2.  ----------------------------------------------------------------- */
    idx_t S = 3u;                      /* smallest possible diagonal (p=3,n=0) */
    while ( (uint64_t)(((__uint128_t)(S - 2u) * (S - 1u)) / 2u) < n ) ++S;

    /* --- Number of entries before this diagonal ----------------------------------------------- */
    idx_t prev = (uint64_t)(((__uint128_t)(S - 3u) * (S - 2u)) / 2u);

    /* --- Position within the diagonal (0‑based) ---------------------------------------------- */
    idx_t offset = n - prev - 1u;          /* 0 ≤ offset < S-2 */

    /* --- Recover p and disk index corresponds to this position ------------------------------- */
    idx_t p   = 3u + offset;              /* p ≥ 3                         */
    idx_t disk = S - p;                    /* disk index n≥0                 */

    /* --- Compute the minimal k with binomial(k + p – 2, p – 2) ≥ disk + 1 ------------------ */
    idx_t m        = p - 2u;               /* choose argument m = p-2        */
    idx_t target   = disk + 1u;            /* required binomial value       */
    idx_t k        = 0u;                   /* current k                      */
    seq_t bin      = 1u;                   /* binomial(k+m, m) with k=0 => 1 */

    while (bin < (seq_t)target) {
        ++k;
        // bin(k+m, m) = bin(k-1+m, m) * (k + m) / k
        bin = bin * (seq_t)(k + m) / (seq_t)k;
    }

    /* --- Result is 2^k ----------------------------------------------------------------- */
    return pow_int((seq_t)2, k);
}
