/*
 * A393410  Sum of the largest parts in the partitions of n into 3 parts whose
 *          smallest part is odd.
 * https://oeis.org/A393410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋ , j odd} Σ_{k=j..⌊(n-j)/2⌋} (n-j-k)
 * Range:     u8 n <= 20, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    idx_t jmax = n / 3;               /* upper bound for the smallest part */

    for (idx_t j = 1; j <= jmax; j += 2) {   /* only odd j contribute */
        idx_t d = (n - j) / 2;                /* upper bound for the middle part */
        idx_t m = d - j + 1;                  /* number of terms in the inner sum */

        /* first and last terms of the arithmetic progression of largest parts */
        seq_t first = (seq_t)n - (seq_t)2 * j;
        seq_t last  = (seq_t)n - (seq_t)j - (seq_t)d;

        /* sum of first and last element */
        seq_t sum_first_last = first + last;

        /* sum of (n-j-k) over k=j..d equals m * (first+last) / 2 */
        seq_t part;
        if ((m & 1) == 0)                /* m even – divide m beforehand */
            part = ((seq_t)(m / 2)) * sum_first_last;
        else                              /* m odd – first+last is even */
            part = ((seq_t)m) * (sum_first_last / 2);

        total += part;
    }
    return total;
}
