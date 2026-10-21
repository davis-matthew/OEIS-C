/*
 * A248826  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation of 1 + x + x^2 + ... + x^n to the polynomial
 *          A_k*(x+k)^k for 0 <= k <= n.
 * https://oeis.org/A248826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(r-k) * binom(r+1, k+1) * k^(r-k) where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2 (r is the row
 *            number, k the column index in that row)
 * Range:     i8 n <= 16, i16 n <= 47, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_seq.h"

/* Compute a(n) using the closed‑form formula above. */
seq_t a(idx_t n)
{
    /* Determine the row r by binary search on r(r+1)/2 ≤ n. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Start index of row r. */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;           /* column index within the row */

    /* Handle the trivial cases. */
    if (k == 0) {
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* binomial coefficient binom(r+1, k+1). */
    seq_t bin = binom(r + 1, k + 1);

    /* k^(r-k) - compute using provided big‑integer power routine. */
    seq_t pw = pow_seq((seq_t)k, (idx_t)(r - k));

    /* Apply the sign (-1)^(r-k). */
    seq_t val = bin * pw;
    if ((r - k) & 1) {
        val = -val;
    }
    return val;
}
