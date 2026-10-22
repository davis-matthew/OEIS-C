/*
 * A300453  Irregular triangle read by rows: row n consists of the
 *          coefficients of the expansion of the polynomial (x + 1)^n + x^2 -
 *          1.
 * https://oeis.org/A300453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of x^k in P(m;x) = (x + 1)^m + x^2 – 1,
 *            where the flat index n falls into row m of the triangle T(m,k)
 *            defined by P(m;x). T(m,0) = 0, T(m,1) = m, T(m,2) = binom(m,2) +
 *            1, T(m,k) = binom(m,k) for k ≥ 3. The row lengths are 3 for m =
 *            0,1,2 and (m+1) otherwise
 * Range:     u8 n <= 72, u16 n <= 200, u32 n <= 649, u64 n <= 2379,
 *            u128 n <= 8843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only for bool, not used here */

/* multiplicative binomial coefficient that does not overflow
   for results that fit into seq_t.  For seq_t up to 64 bits
   an __int128 intermediate is used when available. */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;          /* numerator: n-k+1 … n */
        idx_t div = i;                  /* denominator 1 … k */
        /* Use 128‑bit to avoid overflow when seq_t is 64‑bit or less */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
#ifdef __SIZEOF_INT128__
            unsigned __int128 prod = (unsigned __int128)res * (unsigned __int128)num;
            res = (seq_t)(prod / div);
#else
            res = (res * num) / (seq_t)div;
#endif
        } else {
            /* seq_t is arbitrary‑precision; the builtin operators handle it */
            res = (res * num) / (seq_t)div;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t cum   = 0;   /* count of entries before the current row */
    idx_t m     = 0;   /* current row index */

    /* Find the row that contains the n‑th element */
    for (;;) {
        idx_t len = (m <= 2) ? 3 : m + 1;
        if (cum + len > n)
            break;
        cum += len;
        ++m;
    }

    idx_t j = n - cum;          /* column index within row m (0‑based) */

    if (j == 0)
        return (seq_t)0;

    if (j == 1)
        return my_binom(m, 1);

    if (j == 2)
        return my_binom(m, 2) + (seq_t)1;

    return my_binom(m, j);
}
