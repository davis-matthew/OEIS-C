/*
 * A260854  Base-4 representation of a(n) is the concatenation of the base-4
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the base‑4 representations of 1,2,…,n,
 *            n‑1,…,1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

/* Helper: append the base‑4 representation of k to the current result */
static seq_t append_number(seq_t res, idx_t k)
{
    /* Number of base‑4 digits in k */
    idx_t t   = k;
    idx_t len = 0;
    do {
        ++len;
        t /= 4;
    } while (t);

    /* 4^len as a seq_t */
    seq_t mul = pow_uint(4ULL, len);

    return res * mul + (seq_t)k;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t res = 0;

    /* 1 … n */
    for (idx_t k = 1; k <= n; ++k)
        res = append_number(res, k);

    /* n‑1 … 1 */
    for (idx_t k = n; k-- > 1; )
        res = append_number(res, k);

    return res;
}
