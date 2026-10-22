/*
 * A316349  Consider coefficients U(m,L,k) defined by the identity
 *          Sum_{k=1..L} Sum_{j=0..m} A302971(m,j)/A304042(m,j) * k^j *
 *          (T-k)^j = Sum_{k=0..m} (-1)^(m-k) * U(m,L,k) * T^k that holds for
 *          all positive integers L,m,T. This sequence gives 3-column table
 *          read by rows, where the n-th row lists coefficients...
 * https://oeis.org/A316349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the (n‑th) entry of the 3‑column table of coefficients
 *            U(2,L,k). Let L = ceil(n/3) = (n+2)/3, k = (n-1) mod 3. Then
 *            U(2,L,0) = 6*L^5 + 15*L^4 + 10*L^3 U(2,L,1) = 15*L^4 + 30*L^3 +
 *            15*L^2 U(2,L,2) = 10*L^3 + 15*L^2 + 5*L
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   compute_U2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_U2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for 0, return 0 for safety */
    /* L = ceil(n/3) = (n+2)/3 */
    seq_t L = (seq_t)((n + 2) / 3);
    int k = (int)((n - 1) % 3);
    return compute_U2(L, k);
}
