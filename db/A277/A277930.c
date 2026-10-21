/*
 * A277930  Array of coefficients a(k,n) of the formal power series A(k,x)
 *          read by upwards antidiagonals, where A(k,x) =
 *          ((2*k+1)*x+sqrt(1+4*k*(k+1)*x^2))/(1-x^2), k>=0.
 * https://oeis.org/A277930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) is obtained by reading the array a(k,m) defined by * a(k,0)
 *            = 1, * a(k,2t+1) = 2k+1, * a(k,2t+2) =
 *            1-2⋅∑_{i=0..t}Catalan(i)·(−k(k+1))^(i+1) * through upwards
 *            antidiagonals (k+m constant). * The linear index n (0‑based) is
 *            converted to (k,m) by * locating the diagonal s with s(s+1)/2 ≤
 *            n < (s...
 * Range:     i8 n <= 31, i16 n <= 71, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Locate diagonal s and offset j (= column index) */
    idx_t s   = 0;
    idx_t cum = 0;                /* number of elements before diagonal s */
    for (;; ++s)
    {
        idx_t diag_len = s + 1;
        if (cum + diag_len > n)
            break;
        cum += diag_len;
    }
    idx_t j = n - cum;              /* column index m */
    idx_t k = s - j;                /* row index k */

    if (j == 0)                     /* m = 0 */
        return (seq_t)1;

    if (j & 1)                      /* odd column: a(k,2t+1) = 2k+1 */
        return (seq_t)2 * (seq_t)k + (seq_t)1;

    /* even column: m = 2t+2 */
    idx_t t = j / 2 - 1;            /* number of terms in the sum */
    seq_t sum = 0;
    seq_t base = - ((seq_t)k * ((seq_t)k + (seq_t)1));
    seq_t pow  = base;              /* base^(1) */
    for (idx_t i = 0; i <= t; ++i)
    {
        seq_t cat = catalan(i);
        sum += cat * pow;
        if (i < t)                  /* prepare next power */
            pow *= base;
    }
    seq_t result = (seq_t)1 - (seq_t)2 * sum;
    return result;
}
