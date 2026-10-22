/*
 * A320561  Irregular table read by rows: T(n,k) = (2*k+1)^(2*k+1) mod 2^n, 0
 *          <= k <= 2^(n-1) - 1.
 * https://oeis.org/A320561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) = (2*k+1)^(2*k+1) mod 2^r where r = ⌊log2(n-1+1)⌋
 *            + 1 = ⌊log2(n)⌋ + 1 and k = (n-1) - (2^(r-1)-1) (row-major
 *            indexing)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   modpow_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/modpow_pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, guard */

    /* zero‑based index in the concatenated rows */
    idx_t N = n - 1;

    /* determine the row r (1‑based) by finding the small integer r with
       2^(r-1) <= N+1 < 2^r */
    int r = 1;
    while (((idx_t)1 << r) <= N + 1) r++;

    /* start index of this row (zero‑based) */
    idx_t start = (((idx_t)1 << (r - 1)) - 1);

    /* column index k within the row (0 <= k < 2^(r-1)) */
    idx_t k = N - start;

    /* odd base a = 2*k+1, fits in 64 bits for the requested limits */
    idx_t a_val = (idx_t)2 * k + 1;

    /* modulus 2^r */
    seq_t mod = ((seq_t)1 << r);

    /* compute a_val ^ a_val mod 2^r */
    return modpow_pow2((seq_t)a_val, a_val, mod);
}
