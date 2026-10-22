/*
 * A360212  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-5*k,n-3*k).
 * https://oeis.org/A360212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-5*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t const kmax = n / 3;
    seq_t pos = 0;   /* sum of terms with even k */
    seq_t neg = 0;   /* sum of terms with odd  k */

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t a_val = 2 * n - 5 * k;
        idx_t b_val = n - 3 * k;
        seq_t term = binomial(a_val, b_val);

        if ((k & 1) == 0)   /* even k */
            pos += term;
        else                /* odd  k */
            neg += term;
    }

    /* By definition of the sequence, pos ≥ neg */
    return pos - neg;
}
