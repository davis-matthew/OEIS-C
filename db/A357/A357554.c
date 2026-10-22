/*
 * A357554  Triangular array read by rows. For T(n,k) where 1 <= k <= n, start
 *          with x = k and repeat the map x -> floor(n/x) + (n mod x) until an
 *          x occurs that has already appeared, then that is T(n,k).
 * https://oeis.org/A357554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 9120, u16 n <= 9120, u32 n <= 9120, u64 n <= 9120,
 *            u128 n <= 9120, big any n
 * Helpers:   semi_map
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/semi_map.h"
/* Implements A357554:
   The sequence is given by a triangular array T(n,k) (1 ≤ k ≤ n).
   Starting with x = k, repeatedly apply
        x ← floor(n/x) + (n mod x)
   until a value repeats; that repeated value is T(n,k).
   The output sequence a(n) is the row‑by‑row (flattened) list of the
   T(n,k) values.  This implementation computes a(n) directly from n
   by locating the appropriate row and column and then finding the
   cycle entry with Floyd's algorithm.  No pre‑computed table is used. */

seq_t a(idx_t N)
{
    if (N == 0) return 0;   /* not part of the sequence, but safe */

    /* Locate the row r such that r(r+1)/2 ≥ N.
       The search is O(√N). */
    idx_t r = 1;
    idx_t tri = 1;          /* tri = r(r+1)/2 for current r */
    while (tri < N) {
        r++;
        tri += r;
    }

    /* Determine column k within row r. */
    idx_t prev_tri = tri - r;   /* sum up to row r-1 */
    idx_t k = N - prev_tri;     /* 1-indexed column */

    seq_t rn = (seq_t)r;        /* row value in seq_t type */

    /* Find the first repeated value in the orbit of k under the map. */
    seq_t tortoise = semi_map((seq_t)k, rn);
    seq_t hare = semi_map(tortoise, rn);

    while (tortoise != hare) {
        tortoise = semi_map(tortoise, rn);
        hare = semi_map(semi_map(hare, rn), rn);
    }

    /* Locate the start of the cycle (the repeated value). */
    tortoise = (seq_t)k;
    while (tortoise != hare) {
        tortoise = semi_map(tortoise, rn);
        hare = semi_map(hare, rn);
    }

    return tortoise;
}
