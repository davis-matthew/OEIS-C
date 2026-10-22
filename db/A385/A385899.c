/*
 * A385899  Triangle read by rows: T(n, k, m) = binomial(n, k) * k^n * m^k *
 *          (-1)^(n - k) for m = 2.
 * https://oeis.org/A385899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binomial(r,c) * c^r * 2^c * (-1)^(r-c) where r =
 *            floor((sqrt(8*n+1)-1)/2) and c = n - r*(r+1)/2 (triangular
 *            indexing of the rows)
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 41, i64 n <= 113, i128 n <= 292,
 *            big any n
 * Helpers:   big_pow, binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row. */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;

    /* Compute the triangle entry. */
    seq_t bin  = binom(r, c);                      /* C(r,c) */
    seq_t powk = big_pow((seq_t)c, (seq_t)r);      /* c^r */
    seq_t pow2c = pow2(c);                          /* 2^c */

    seq_t term = bin;
    term *= powk;
    term *= pow2c;

    /* Apply the sign factor. */
    if ((r - c) & 1u) {
        term = -term;
    }

    return term;
}
