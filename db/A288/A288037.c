/*
 * A288037  Number of (undirected) paths in the n-web graph.
 * https://oeis.org/A288037
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-104 + 89*2^n - 52*n - 8*n^2) / 4
 * Range:     u8 none, u16 n <= 8, u32 n <= 23, u64 n <= 53, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Works for n >= 3. For n < 3 return 0 (undefined region). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3u)
        return 0;

    /* Compute 2^n in a seq_t variable. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    seq_t nn = (seq_t)n;                      /* cast index to seq_t */

    /* Compute the polynomial components: 89*2^n, 52*n, 8*n^2. */
    seq_t term1 = 89 * pow2;                  /* 89*2^n   */
    seq_t term2 = 52 * nn;                    /* 52*n     */
    seq_t term3 = 8 * nn * nn;                /* 8*n^2    */

    /* Numerator of the formula: -104 + term1 - term2 - term3. */
    seq_t numerator = term1 - term2 - term3 - 104u;

    /* Compute n * numerator / 4 without overflow when the final
       result fits in seq_t.  Split n into n/4 and n%4 parts. */
    idx_t n_div4 = n >> 2;          /* n / 4 (integer division) */
    idx_t n_mod4 = n & 3 ;          /* n % 4                */

    seq_t part1 = (seq_t)n_div4 * numerator;          /* (n/4)*num    */
    seq_t part2 = ((seq_t)n_mod4 * numerator) / 4;    /* (n%4)*num/4 */

    return part1 + part2;
}
