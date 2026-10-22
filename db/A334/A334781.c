/*
 * A334781  Array read by antidiagonals: T(n,k) = Sum_{i=1..n}
 *          binomial(1+i,2)^k.
 * https://oeis.org/A334781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence obtained by reading the array T(n,k) =
 *            Sum_{i=1..n} binomial(i+1,2)^k by antidiagonals. For index n we
 *            find the diagonal d where d(d+1)/2 <= n < (d+1)(d+2)/2, then
 *            n_val = n - d(d+1)/2 (row index in T) k_val = d - n_val (column
 *            index in T) and a(n) = T(n_val, k_val)
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 108, u64 n <= 236,
 *            u128 n <= 568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return x^exp as seq_t using repeated multiplication. */
static seq_t pow_seq_local(seq_t x, idx_t exp)
{
    seq_t r = 1;
    for (idx_t e = 0; e < exp; ++e)
        r *= x;
    return r;
}

/* Compute T(n,k) = sum_{i=1..n} binomial(i+1,2)^k. */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq = (seq_t)i;
        seq_t tri = (i_seq * (i_seq + 1)) / 2;   /* binomial(i+1,2) */
        sum += pow_seq_local(tri, k);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d (0‑based) containing index n. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    idx_t first_index = (d * (d + 1)) / 2;
    idx_t offset     = n - first_index;   /* position within this diagonal */
    idx_t n_val      = offset;            /* row index in T */
    idx_t k_val      = d - offset;        /* column index in T */

    return T_value(n_val, k_val);
}
