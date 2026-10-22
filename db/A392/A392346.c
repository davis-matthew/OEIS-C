/*
 * A392346  The smallest k >= 0 that can be represented as a linear
 *          combination of 1, 2, ..., n with coefficients +-1 and that cannot
 *          be represented using 1, 2, ..., m with 1<=m<n.
 * https://oeis.org/A392346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is the first index with its parity of sum(1..n) ->
 *            a(n) = (parity==even ? 0 : 1) else a(n) = sum(1..m) + 2 where m
 *            < n is the largest index with the same parity. For a given n,
 *            this reduces to the simple closed‑form below
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* t_n = n(n+1)/2 */
    seq_t t_n = (seq_t)n * ((seq_t)n + 1) / 2;

    /* parity of t_n : 0 = even, 1 = odd */
    int parity;
    idx_t r = n % 4;
    if (r == 0 || r == 3)
        parity = 0;    /* even */
    else
        parity = 1;    /* odd */

    /* Handle first occurrence of each parity */
    if (parity == 0) {                /* even parity */
        if (n == 3)                   /* first even sum index */
            return 0;
        /* largest previous index with even t_n */
        idx_t m = (r == 0) ? (n - 1) : (n - 3);
        seq_t t_m = (seq_t)m * ((seq_t)m + 1) / 2;
        return t_m + 2;
    } else {                           /* odd parity */
        if (n == 1)                   /* first odd sum index */
            return 1;
        /* largest previous index with odd t_n */
        idx_t m = (r == 2) ? (n - 1) : (n - 3);
        seq_t t_m = (seq_t)m * ((seq_t)m + 1) / 2;
        return t_m + 2;
    }
}
