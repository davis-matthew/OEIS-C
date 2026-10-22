/*
 * A394466  Triangle read by rows: T(n,k) = numerator(CF(n,k)), where CF(n,k)
 *          = (1/k!) * Sum_{j=0..k} (-1)^j * binomial(k,j) * (2+k/2-j)^n.
 * https://oeis.org/A394466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = numerator of CF(row, col) where row and col are obtained
 *            by reading A394466 as a triangle by rows. Implementation uses
 *            the definition of CF from the OEIS entry: CF(n,k) = (1/k!) *
 *            Σ_{j=0..k} (-1)^j * binomial(k,j) * (2 + k/2 - j)^n The triangle
 *            entry T(n,k) is the numerator of CF(n,k) in l...
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial, factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case for the very first element */
    if (n == 0) return (seq_t)1;

    /* ------------------------------------------------------------
       Find the row `row` such that S(row-1) <= n < S(row),
       where S(m) = (m+1)(m+2)/2 is the cumulative number of terms
       up to row m.
       ------------------------------------------------------------ */
    idx_t row;
    {
        idx_t low = 0, high = 1;
        while ( ((__uint128_t)(high + 1) * (high + 2)) / 2 <= n )
            high <<= 1;                     /* find an upper bound   */
        while (low + 1 < high) {
            idx_t mid = low + (high - low) / 2;
            if ( ((__uint128_t)(mid + 1) * (mid + 2)) / 2 <= n )
                low = mid;
            else
                high = mid;
        }
        row = high;                          /* minimal row with S(row) > n */
    }

    /* Determine the column inside that row */
    idx_t start = row * (row + 1) / 2;      /* S(row-1) */
    idx_t k = n - start;                    /* column index (0‑based) */

    /* ------------------------------------------------------------
       Compute the reduced fraction CF(row,k) and return its numerator.
       ------------------------------------------------------------ */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    seq_t kFact   = factorial(k);

    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin = binomial(k, j);             /* binomial coefficient */
        seq_t term_abs;
        bool term_pos;

        if (k % 2 == 0) {                      /* k even: integer base */
            int base_int = 2 + (int)(k / 2) - (int)j;
            seq_t base_abs = (base_int >= 0) ? (seq_t)base_int
                                             : (seq_t)(-base_int);
            term_abs = bin * big_pow(base_abs, (seq_t)row);

            if (base_int < 0)                  /* negative base contributes */
                term_pos = ((j + row) % 2 == 0);
            else
                term_pos = ((j % 2) == 0);
        }
        else {                                  /* k odd: half‑integer base */
            int m       = (int)(k - 1) / 2;
            int s       = m - (int)j;                    /* m-j */
            int base_num = 2 * s + 5;                   /* odd numerator */
            seq_t base_abs = (base_num >= 0) ? (seq_t)base_num
                                              : (seq_t)(-base_num);
            term_abs = bin * big_pow(base_abs, (seq_t)row);

            if (base_num < 0)                    /* negative base */
                term_pos = ((j + row) % 2 == 0);
            else
                term_pos = ((j % 2) == 0);
        }

        if (term_pos)
            pos_sum += term_abs;
        else
            neg_sum += term_abs;
    }

    /* Absolute value of the sum */
    seq_t numerator_abs;
    if (pos_sum >= neg_sum)
        numerator_abs = pos_sum - neg_sum;
    else
        numerator_abs = neg_sum - pos_sum;

    /* Full denominator before reduction */
    seq_t denom = kFact;
    if (k % 2 == 1)                               /* factor 2^row for odd k */
        denom *= big_pow((seq_t)2, (seq_t)row);

    /* Reduce the fraction */
    seq_t g = gcd(numerator_abs, denom);
    seq_t result = numerator_abs / g;
    return result;   /* numerator of the reduced CF(row,k) */
}
