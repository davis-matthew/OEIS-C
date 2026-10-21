/*
 * A298592  Triangle read by rows: T(n,k) = number of parking functions of
 *          length n whose lead number is k.
 * https://oeis.org/A298592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = Σ_{j=k..n} C(n-1,j-1) *
 *            j^(j-2) * (n+1-j)^(n-1-j)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static seq_t parking_value(idx_t row, idx_t col)
{
    seq_t total = 0;
    for (idx_t j = col; j <= row; ++j)
    {
        seq_t c = binomial(row - 1, j - 1);

        seq_t a;
        if (j > 1)
            a = big_pow((seq_t)j, (seq_t)(j - 2));
        else
            a = 1;

        seq_t b;
        if (row > j)
            b = big_pow((seq_t)(row + 1 - j), (seq_t)(row - 1 - j));
        else
            b = 1;

        total += c * a * b;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t row = 0;
    while (((row + 1) * (row + 2)) / 2 < n)
        ++row;
    ++row;                               /* row satisfies row*(row+1)/2 >= n */

    idx_t prev_suffix = (row - 1) * row / 2;
    idx_t col = n - prev_suffix;          /* column within the row */

    return parking_value(row, col);
}
