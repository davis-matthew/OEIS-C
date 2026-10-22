/*
 * A395701  Triangle read by rows: T(n, k) = binomial(n, k) * k^k.
 * https://oeis.org/A395701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * k^k, where r = max{m | m*(m+1)/2 ≤ n} and
 *            k = n - r*(r+1)/2
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 64, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t start = 0;          /* starting index of the current row */

    /* Move to the row containing index n */
    while (start + r + 1 <= n) {
        start += r + 1;       /* skip the current row */
        r++;
    }

    idx_t k = n - start;      /* column within the found row */

    seq_t bin = binomial(r, k);
    seq_t pw  = pow_int((seq_t)k, k);
    return bin * pw;
}
