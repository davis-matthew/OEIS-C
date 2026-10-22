/*
 * A334968  Number of possible sums of subsequences (not necessarily
 *          contiguous) of the n-th composition in standard order (A066099).
 * https://oeis.org/A334968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of distinct subset sums of the n-th composition in
   standard order (graded reverse-lexicographic). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 yields the empty composition → only sum 0 */
    if (n == 0)
        return (seq_t)1;

    /* Gather positions (1‑based) of set bits in n, in increasing order.  */
    int pos[64];          /* maximum number of set bits in a 64‑bit integer */
    int m = 0;            /* number of set bits (parts of the composition) */
    for (int i = 0; i < 64; ++i)
        if (n & ((idx_t)1 << i))
            pos[m++] = i + 1;          /* positions are 1‑based */

    /* The highest position equals the total sum of the composition. */
    int S = pos[m - 1];   /* max sum; S <= 64 for a 64‑bit integer */

    /* Compute the composition parts as differences of consecutive positions.
       These are in reverse order but that does not affect the DP. */
    int part[64];
    int prev = 0;
    for (int j = 0; j < m; ++j) {
        int cur = pos[j];
        part[j] = cur - prev;  /* diff or first part */
        prev = cur;
    }

    /* DP for reachable subset sums: reach[s] = true iff sum s is achievable.
       Since S ≤ 64 we can use a fixed-size array. */
    bool reach[65] = {0};
    reach[0] = true;

    for (int j = 0; j < m; ++j) {
        int v = part[j];          /* value of current part */
        for (int s = S; s >= v; --s)
            if (reach[s - v])
                reach[s] = true;
    }

    /* Count reachable sums. */
    seq_t count = 0;
    for (int s = 0; s <= S; ++s)
        if (reach[s])
            ++count;

    return count;
}
