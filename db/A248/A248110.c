/*
 * A248110  Table read by rows: n-th row contains the q successors of n, where
 *          q = A007953(n), the digit sum of n in decimal representation.
 * https://oeis.org/A248110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = i + (n – S(i-1)) where S(k)=Σ_{t=1}^k digit_sum(t) and i
 *            is the smallest integer with S(i)≥n
 * Range:     u8 n <= 2270, u16 n <= 9280, u32 n <= 9280, u64 n <= 9280,
 *            u128 n <= 9280, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* The sequence is defined by concatenating rows: row i contains q=A007953(i) successive integers
       starting at i+1.  The n‑th term is the member at position (n – S(i‑1)) within that row.  */
    if (n == 0)               /* out‑of‑range guard – not required by OEIS offset */
        return 0;

    idx_t cumulative = 0;     /* number of terms produced so far */
    idx_t i = 1;              /* current row number */

    seq_t ds; /* digit sum of i as seq_t */

    while (1) {
        ds = digit_sum((seq_t)i);          /* digit sum of the current row index */
        cumulative += (idx_t)ds;
        if (cumulative >= n)
            break;
        ++i;
    }

    idx_t previous_cumulative = cumulative -
        (idx_t)digit_sum((seq_t)i);         /* cumulative up to row i‑1 */
    idx_t offset = n - previous_cumulative;/* 1‑based position within the row */

    return (seq_t)i + (seq_t)offset;
}
