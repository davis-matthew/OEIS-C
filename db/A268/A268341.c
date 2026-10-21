/*
 * A268341  Triangle T(n,k) = Degree of vertex k in the unitary addition
 *          Cayley graph Gn, 0<=k<=n-1, with T(1,0)=0.
 * https://oeis.org/A268341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle T(n,k) = φ(n) if n is even, else T(n,k) = φ(n) –
 *            [gcd(n,k)==1]
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t N)
{
    /* Step 1: find the row r such that r*(r+1)/2 ≥ N. */
    idx_t low = 0, high = 1;
    /* Exponentially increase high until it covers N */
    while ((idx_t)((__int128)high * (high + 1) / 2) < N) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cum = (idx_t)((__int128)mid * (mid + 1) / 2);
        if (cum < N)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;                 /* r is the row number (1‑based) */

    /* Special case T(1,0) = 0 */
    if (r == 1)
        return (seq_t)0;

    /* Compute the column index k (0‑based) within row r. */
    idx_t prev = (idx_t)((__int128)(r - 1) * r / 2);  /* terms before row r */
    idx_t k = N - prev - 1;

    /* Compute φ(r). */
    seq_t phi_r = phi(r);

    /* Determine the final value. */
    if ((r & 1) == 0) {                 /* r even */
        return phi_r;
    } else {
        /* r odd: subtract 1 iff gcd(r,k)==1 */
        if (gcd((seq_t)r, (seq_t)k) == (seq_t)1)
            return phi_r - (seq_t)1;
        else
            return phi_r;
    }
}
