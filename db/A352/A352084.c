/*
 * A352084  Integers m such that wt(m) divides wt(m^2) where wt(m) =
 *          A000120(m) is the binary weight of m.
 * https://oeis.org/A352084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 8080, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A352084
   Formula: a(n) = the n-th integer m such that popcount(m) divides popcount(m^2) */
/* The algorithm scans positive integers, counts the binary weight
   of m and of m^2 (both computed in 128‑bit arithmetic), and
   returns the n‑th m satisfying the divisibility condition. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/popcnt128.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    unsigned __int128 m = 1;            /* current candidate */
    while (1) {
        size_t w  = popcnt128(m);            /* wt(m)   */
        unsigned __int128 sq = m * m;
        size_t ws = popcnt128(sq);           /* wt(m^2) */

        /* If m is 0 the divisor would be 0; m starts at 1 so this
           check is unnecessary but kept for safety when m overflows
           back to 0 (e.g. with 8‑bit seq_t). */
        if (w != 0 && (ws % w) == 0) {
            ++found;
            if (found == n) {
                return (seq_t)m;             /* truncate to seq_t size */
            }
        }
        ++m;
    }
}
