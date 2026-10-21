/*
 * A286895  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 7 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A286895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = (7^(m*n) + 3*7^(m*n/2))/4 if n even & m even (7^(m*n) +
 *            7^((m*n+n)/2) + 2*7^(m*n/2))/4 if n even & m odd (7^(m*n) +
 *            7^((m*n+m)/2) + 2*7^(m*n/2))/4 if n odd & m even (7^(m*n) +
 *            7^((m*n+n)/2) + 7^((m*n+m)/2)+7^((m*n+1)/2))/4 a(n) is the entry
 *            read by rows; a(0)=T(0,0)=1. The function accep...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* locate row r and column c from linear index n */
    idx_t r = 0;
    /* find first r such that r*(r+1)/2 > n; then r-1 is the desired row */
    while ( (r * (r + 1)) / 2 <= n ) {
        ++r;
    }
    if (r == 0) {          /* only happens for n = 0 */
        r = 0;
    } else {
        --r;                /* actual row index */
    }

    idx_t first = (r * (r + 1)) / 2;   /* first linear index of this row */
    idx_t c = n - first;               /* column index within the row */

    idx_t mn = r * c;                  /* m*n */

    bool n_even = ((r & 1) == 0);
    bool m_even = ((c & 1) == 0);

    seq_t term1 = big_pow((seq_t)7, (seq_t)mn);   /* 7^(mn) */

    if (n_even) {
        if (m_even) {
            /* even n, even m: (7^(mn) + 3*7^(mn/2)) / 4 */
            seq_t term2 = big_pow((seq_t)7, (seq_t)(mn / 2));
            return (term1 + 3 * term2) / 4;
        } else {
            /* even n, odd m: (7^(mn) + 7^((mn+n)/2) + 2*7^(mn/2)) / 4 */
            seq_t term2 = big_pow((seq_t)7, (seq_t)( (mn + r) / 2));
            seq_t term3 = big_pow((seq_t)7, (seq_t)(mn / 2));
            return (term1 + term2 + 2 * term3) / 4;
        }
    } else {   /* n is odd */
        if (m_even) {
            /* odd n, even m: (7^(mn) + 7^((mn+m)/2) + 2*7^(mn/2)) / 4 */
            seq_t term2 = big_pow((seq_t)7, (seq_t)( (mn + c) / 2));
            seq_t term3 = big_pow((seq_t)7, (seq_t)(mn / 2));
            return (term1 + term2 + 2 * term3) / 4;
        } else {
            /* odd n, odd m: (7^(mn) + 7^((mn+n)/2) + 7^((mn+m)/2) + 7^((mn+1)/2))/4 */
            seq_t term2 = big_pow((seq_t)7, (seq_t)( (mn + r) / 2));
            seq_t term3 = big_pow((seq_t)7, (seq_t)( (mn + c) / 2));
            seq_t term4 = big_pow((seq_t)7, (seq_t)( (mn + 1) / 2));
            return (term1 + term2 + term3 + term4) / 4;
        }
    }
}
