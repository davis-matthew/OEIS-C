/*
 * A323953  Regular triangle read by rows where T(n, k) is the number of ways
 *          to split an n-cycle into singletons and connected subsequences of
 *          sizes > k.
 * https://oeis.org/A323953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of a triangle read by rows. * The triangle
 *            T has rows numbered n = 1,2,3,… with length n. * * For k = 0 the
 *            row ends with 1: T(n,0) = 1. * For k >= 1 the entry is defined
 *            by * * T(n,k) = 2 – n + Σ_{i=1..⌊n/k⌋} n·binomial(n – i·k + i –
 *            1, 2·i – 1) / i. * * The sequence a(n) i...
 * Range:     u8 n <= 36, u16 n <= 136, u32 n <= 528, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t triangle_value(idx_t n, idx_t k)
{
    /* k = 0 gives the last entry of the row */
    if (k == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t max_i = n / k;      /* floor(n/k) */

    for (idx_t i = 1; i <= max_i; ++i) {
        /* a = n - i*k + i - 1, b = 2*i - 1 */
        idx_t a = n - i * k + i - 1;
        idx_t b = 2 * i - 1;
        seq_t bin = binomial(a, b);
        seq_t term = ((seq_t)n * bin) / (seq_t)i;
        sum += term;
    }

    /* res = 2 - n + sum; avoid signed underflow */
    seq_t res = sum;
    res -= (seq_t)n;
    res += (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* below offset; value unused */

    /* Convert to 0‑based index                                */
    idx_t pos = n - 1;

    /* Binary search for the row r such that r(r+1)/2 > pos */
    idx_t low = 1, high = 1;
    while (((__uint128_t)high * (high + 1)) / 2 <= pos)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__uint128_t)mid * (mid + 1)) / 2 > pos)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                         /* row number (1‑based) */

    /* Position of the first element of row r in the flattened list */
    idx_t row_start = (idx_t)(((__uint128_t)r * (r - 1)) / 2);

    /* Index within the row (0‑based)                            */
    idx_t j = pos - row_start;

    if (j == r - 1)                     /* last element of the row */
        return (seq_t)1;                /* T(r,0) */

    /* Map j to k : first element of the row has k = 1            */
    idx_t k = j + 1;
    return triangle_value(r, k);
}
