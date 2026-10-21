/*
 * A273171  Triangle for numerators of coefficients for integrated odd powers
 *          of cos(x) in terms sin((2*m+1)*x).
 * https://oeis.org/A273171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) is the numerator of the reduced fraction R(r,m) =
 *            binomial(2r+1, r-m) / (2^(2r) * (2m+1)), where (r,m) is the
 *            (row,column) obtained from the triangular numbering: r*(r+1)/2 ≤
 *            n < (r+1)(r+2)/2
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Compute a(n) for the OEIS sequence A273171 */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((__int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    /* Column index m within the row */
    idx_t row_start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t m = n - row_start;

    /* Compute the raw binomial coefficient */
    seq_t num = binomial(2 * r + 1, r - m);

    /* Count the number of twos in num (exp2). */
    idx_t exp2 = 0;
    seq_t tmp = num;
    while ((tmp & 1) == 0)
    {
        tmp >>= 1;
        ++exp2;
    }

    /* Cancel as many 2‑factors as the denominator supplies. */
    idx_t cancel = exp2 < (2 * r) ? exp2 : (2 * r);
    seq_t num_after2 = num >> cancel;

    /* Reduce odd part: denominator odd factor is (2*m+1) */
    seq_t odd = (seq_t)(2 * m + 1);
    seq_t g = gcd(num_after2, odd);

    /* Final numerator after full reduction */
    return num_after2 / g;
}
