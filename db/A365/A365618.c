/*
 * A365618  Table read by antidiagonals: T(n, k) = A000120(n) + A000120(k).
 * https://oeis.org/A365618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   a(n) = popcount(i) + popcount(d-i) where d is the smallest
 *            integer with (d+1)(d+2)/2 > n and i = n - d(d+1)/2 (0 ≤ i ≤ d)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required algorithmic details:
   • For index n compute the diagonal d by comparing
     cumulative counts with 64‑bit integers; use __uint128_t
     to avoid overflow while multiplying.
   • The offset inside the diagonal is just the difference
     between n and the number of terms before this diagonal.
   • Compute popcount of the two numbers with the GCC/Clang
     builtin popcount. */
 
#include <stdint.h> /* for uint64_t, etc. */

/* popcount of a 64‑bit integer */
static inline unsigned int popcount_idx(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

/* cumulative number of terms up to and including diagonal d :
   total(d) = (d+1)(d+2)/2
   cumulative before diagonal d : prev(d) = d(d+1)/2  */
static inline idx_t cumulative_before(idx_t d)
{
    return (idx_t)(((__uint128_t)d * (d + 1)) / 2);
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that total(d) > n */
    idx_t d = 0;
    while (((__uint128_t)(d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Offset inside this diagonal (0 … d) */
    idx_t prev = cumulative_before(d);
    idx_t i   = n - prev;        /* row index on this antidiagonal */
    idx_t k   = d - i;          /* column index on this antidiagonal */

    /* T(i,k) = popcount(i) + popcount(k) */
    return (seq_t)(popcount_idx(i) + popcount_idx(k));
}
