/*
 * A397713  Triangle read by rows: T(n,k) is the number of binary digits in
 *          binomial(n,k).
 * https://oeis.org/A397713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial, bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑formula:  a(n) is the number of binary digits of the binomial coefficient
   C(r,k), where r = floor((sqrt(8n+1)-1)/2) is the row containing index n,
   and k = n - r*(r+1)/2 is the column in that row. */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    idx_t cum = 0;                   /* cumulative elements before row r */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }
    /* Position within the found row. */
    idx_t k = n - cum;                /* 0 <= k <= r */

    /* Compute the binomial coefficient and the number of binary digits. */
    seq_t val = binomial(r, k);
    return (seq_t)bitlen_seq_t(val);
}
