/*
 * A350698  Consider the positive squares summing to n as obtained by the
 *          greedy algorithm; a(n) is the least of these squares.
 * https://oeis.org/A350698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least square in the greedy decomposition of n into *
 *            squares. For n a perfect square a(n)=n; otherwise a(n) is the
 *            smallest * square that appears when repeatedly subtracting the
 *            largest square ≤ * the remainder
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(sqrt(n)) for a 64‑bit unsigned integer without overflow
 * by binary search.  This works for any idx_t that is a 64‑bit integral
 * type. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 0xFFFFFFFFULL;      /* maximal sqrt(2^64-1) */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid)          /* mid*mid <= n  (no overflow) */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}
 
seq_t a(idx_t n)
{
    idx_t remaining = n;
    seq_t min_sq = (seq_t)n;          /* worst case: the whole n */
 
    while (remaining != 0) {
        idx_t k   = isqrt_u64(remaining);   /* largest k with k^2 ≤ remaining */
        idx_t sq  = k * k;
        if ((seq_t)sq < min_sq)
            min_sq = (seq_t)sq;
        remaining -= sq;
    }
    return min_sq;
}
