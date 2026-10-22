/*
 * A395456  Triangle read by rows: T(n,k) = (1/(2k)!) * Sum_{j=0..2k} (-1)^j *
 *          binomial(2k,j) * (2+k-j)^(2n).
 * https://oeis.org/A395456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = flattening of the triangle T(n,k) = (1/(2k)!) *
 *            Σ_{j=0}^{2k} (-1)^j * binomial(2k,j) * (2+k-j)^(2n); The
 *            sequence a(n) lists the values T(r,c) for r = 0,1,2,… and c =
 *            0…r in row‑major order
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 30, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) ++r;
    --r;

    /* Column index in that row. */
    idx_t before = r * (r + 1) / 2;
    idx_t c = n - before;

    /* (2c)! factorial. */
    seq_t fact = factorial(2 * c);

    /* Sum with alternating signs and divide by (2c)! */
    seq_t pos = 0, neg = 0;
    for (idx_t j = 0; j <= 2 * c; ++j) {
        seq_t bin = binomial(2 * c, j);
        int64_t base_int = (int64_t)2 + (int64_t)c - (int64_t)j;
        seq_t base_abs = (base_int >= 0) ? (seq_t)base_int : (seq_t)(-base_int);
        seq_t pow_val = big_pow(base_abs, (seq_t)(2 * r));
        seq_t term = bin * pow_val;
        if (j & 1) {
            neg += term;
        } else {
            pos += term;
        }
    }
    seq_t numer = pos - neg;         /* pos >= neg by construction */
    return numer / fact;             /* exact division */
}
