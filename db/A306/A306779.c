/*
 * A306779  Table read by rows: T(n,k) is the length of the longest
 *          non-intersecting loop starting at (0,0) on the n X k torus
 *          consisting of steps up and to the right, 1 <= k <= n.
 * https://oeis.org/A306779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy, walk
 * Formula:   a(n) is the value T(r,k) from the A306779 table, where r and k
 *            are the row and column numbers obtained by writing the table by
 *            rows (row 1 has 1 entry, row 2 has 2, etc.). The definition of
 *            T(r,k) used is T(r,k) = r*k - min{ (r*x) mod k : 0 ≤ x < k,
 *            gcd(k - x, floor(r*x/k)) = 1 }
 * Range:     u8 n <= 135, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Find the row r (1‑based) such that r*(r+1)/2 ≥ i */
static idx_t row_from_index(idx_t i)
{
    idx_t low = 1, high = i;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= i)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute T(n,k) using the residue‑minimisation formula. */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t best = 0;
    seq_t base = (seq_t)n * (seq_t)k;

    for (idx_t x = 0; x < k; ++x) {
        unsigned __int128 prod = (unsigned __int128)n * x;
        seq_t res  = (seq_t)(prod % k);
        seq_t part1 = (seq_t)(k - x);
        seq_t part2 = (seq_t)(prod / k);

        if (gcd(part1, part2) == 1) {
            seq_t cand = base - res;
            if (cand > best)
                best = cand;
        }
    }
    return best;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* unused but safe for 0‑based callers */

    idx_t r   = row_from_index(n);
    unsigned __int128 start = (unsigned __int128)(r - 1) * r / 2;
    idx_t k   = n - (idx_t)start;   /* column index (1‑based) */

    return T_value(r, k);
}
