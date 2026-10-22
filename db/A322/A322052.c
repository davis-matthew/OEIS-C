/*
 * A322052  Number of decimal strings of length n that contain a specific
 *          string xy where x and y are distinct digits.
 * https://oeis.org/A322052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^n - (A(n)+B(n)) where A(1)=1, B(1)=9 and
 *            A(k+1)=A(k)+B(k), B(k+1)=8*A(k)+9*B(k); (A,B count length‑k
 *            strings avoiding the pattern xy)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of range – defined to be 0 */

    /* Use seq_t arithmetic directly to avoid overflow */
    seq_t A = 1;          /* strings of length 1 ending with x */
    seq_t B = 9;          /* strings of length 1 ending with any digit ≠ x */

    for (idx_t len = 2; len <= n; ++len) {
        seq_t newA = A + B;          /* state 1 (last digit is x) */
        seq_t newB = 8 * A + 9 * B;   /* state 0 (last digit is not x) */
        A = newA;
        B = newB;
    }

    seq_t avoid = A + B;     /* total avoid counts */
    seq_t total = pow10(n);  /* total number of length‑n strings */

    return total - avoid;    /* strings that contain xy at least once */
}
