/*
 * A317505  Triangle read by rows: T(0,0) = 1; T(n,k) = - T(n-1,k) - 2
 *          T(n-3,k-1) for k = 0..floor(n/3); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   a(n) is the row‑major enumeration of the triangle * T(r,k) =
 *            (-1)^(r - 3k) * 2^k * C(r - 2k, k) * where the triangle is read
 *            by rows
 * Range:     i8 n <= 32, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   binom, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that a(n) = T(r,k) in row‑major order. */
    idx_t cum = 0;   /* cumulative number of entries up to but not including row r */
    idx_t r = 0;     /* current row */

    while (1) {
        idx_t len = r / 3 + 1;              /* number of entries in row r */
        if (cum + len > n)
            break;                         /* n lies in this row */
        cum += len;
        ++r;
    }

    idx_t k = n - cum;                     /* column within row r */
    idx_t parity = r - 3 * k;              /* determines the sign */

    /* Compute the binomial coefficient C(r-2k, k). */
    seq_t bin = binom(r - 2 * k, k);

    /* Compute 2^k. */
    seq_t pow2k = pow2(k);

    /* Multiply 2^k by the binomial coefficient. */
    seq_t val = seq_mul(pow2k, bin);

    /* Apply the alternating sign. */
    if (parity & 1)          /* parity odd  -> negative sign */
        val = -val;

    return val;
}
