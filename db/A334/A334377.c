/*
 * A334377  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of k into distinct parts p such that 2 <= p <= n.
 * https://oeis.org/A334377
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the flattened list of the coefficients of
 *          ∏_{p=2..row} (1 + x^p) where “row” is the row index
 *          (row 2 corresponds to the first row of the triangle).
 *          The sequence lists the coefficients row by row,
 *          starting with n = 2.
 *
 * This implementation finds the row containing the requested
 * coefficient, builds the corresponding subset‑sum table, and
 * returns the desired entry.  The max row for n ≤ 10000 is 39,
 * so a small fixed‑size array is sufficient.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* offset is 2, value below is unspecified */
        return 0;

    /* Convert the flattened index to a zero‑based offset */
    uint64_t m = (uint64_t)n - 2;      /* m ≥ 0 */

    /* Find the smallest row i such that cumulative(i) > m.
       cumulative(i) = i(i+1)(i+2)/6 - 1  (total terms up to row i) */
    idx_t i = 2;
    while (1) {
        uint64_t cum = (uint64_t)i * (i + 1) * (i + 2) / 6 - 1;
        if (cum > m)
            break;
        ++i;
    }

    /* Previous cumulative total */
    uint64_t cumPrev = 0;
    if (i > 2)
        cumPrev = (uint64_t)(i - 1) * i * (i + 1) / 6 - 1;

    /* Position within the chosen row */
    idx_t k = m - cumPrev;

    /* Length of the row: number of coefficients */
    size_t len = (size_t)i * (i + 1) / 2;        /* <= 780 for i <= 39 */

    /* Use a fixed-size array on the stack: max possible row size is 800 */
    seq_t dp[800];
    for (size_t s = 0; s < len; ++s)      /* initialise to zero */
        dp[s] = 0;
    dp[0] = 1;                              /* empty subset */

    /* Build the generating function ∏_{p=2..i} (1 + x^p) */
    for (idx_t p = 2; p <= i; ++p) {
        if ((size_t)p >= len)
            continue;
        for (int s = (int)len - 1; s >= (int)p; --s)
            dp[s] += dp[s - p];
    }

    return dp[(size_t)k];
}
