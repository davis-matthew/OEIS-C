/*
 * A388733  Triangle read by rows: T(n,k) = numerators of "across the board"
 *          style tournament payouts.
 * https://oeis.org/A388733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) = r! * Σ_{j=c}^{r} 1/j where r is the smallest
 *            integer with r*(r+1)/2 ≥ n and c = n - r*(r-1)/2 (row r has r
 *            entries, indexing starts at 1)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine uses only integer arithmetic on seq_t and idx_t.
   It computes the factorial of r by simple multiplication, which
   is fast for r ≤ 141 (for n ≤ 10 000).  No external utilities
   are required. */

#include <math.h>   /* for sqrt() */
#include <stddef.h> /* for size_t */

/* Compute the cumulative number of terms up to row r: r*(r+1)/2 */
static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* Return the centered triangle term for flattened index n. */
seq_t a(idx_t n)
{
    /* Undefined for n = 0; return 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    /* Find r such that triangular(r) ≥ n.  Use the quadratic formula. */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1)) - 1.0) / 2.0);
    /* Adjust if we fell short because of rounding. */
    if (triangular(r) < n)
        ++r;

    /* Column index in row r (1‑based). */
    idx_t c = n - triangular(r - 1);

    /* Compute r! . */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= r; ++i)
        fact = fact * (seq_t)i;

    /* Sum fact / j for j = c … r.  Each division is exact. */
    seq_t sum = (seq_t)0;
    for (idx_t j = c; j <= r; ++j)
        sum += fact / (seq_t)j;

    return sum;
}
