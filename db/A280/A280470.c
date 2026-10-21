/*
 * A280470  Triangle A106534 with reversed rows.
 * https://oeis.org/A280470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{k=0..c} binomial(c,k) * Catalan(r-k), where r
 *            satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and c = n - r*(r+1)/2 (0
 *            ≤ c ≤ r)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that triangular number r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    seq_t n_seq = (seq_t)n;
    for (;;) {
        seq_t next_tri = (seq_t)(r + 1) * (r + 2) / 2;
        if (next_tri > n_seq)
            break;
        ++r;
    }

    /* Compute column index c within row r */
    seq_t start_tri = (seq_t)r * (r + 1) / 2;
    seq_t c_seq = n_seq - start_tri;
    idx_t c = (idx_t)c_seq;   /* c ≤ r, fits in idx_t */

    /* Compute the triangle entry by summing binomial*Catalan terms */
    seq_t sum = 0;
    for (idx_t j = 0; j <= c; ++j) {
        sum += binomial(c, j) * catalan(r - j);
    }
    return sum;
}
