/*
 * A287745  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 342", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A287745):
   Let k = n/4 and r = n % 4 (0 ≤ r ≤ 3).
   Let Pk   = 4^k.
   Then A_k = (Pk * (4*Pk - 1)) / 3   = (4^k * (4^{k+1} - 1)) / 3.

   a(n) = {
             A_k,                     if r = 0
             2*A_k,                   if r = 1 or 2
             4*A_k                    if r = 3
          }
*/

seq_t a(idx_t n) {
    /* Compute k = n / 4 */
    idx_t k = n / 4;
    /* Compute Pk = 4^k by repeated multiplication */
    seq_t Pk = 1;
    for (idx_t i = 0; i < k; ++i) {
        Pk = Pk * (seq_t)4;
    }
    /* Compute A_k = (Pk * (4*Pk - 1)) / 3 */
    seq_t temp = (Pk << 2) - (seq_t)1;   /* 4*Pk - 1 */
    seq_t A_k  = (Pk * temp) / (seq_t)3;

    /* Determine factor based on r = n % 4 */
    idx_t r = n % 4;
    seq_t factor;
    if (r == 0) {
        factor = (seq_t)1;
    } else if (r == 3) {
        factor = (seq_t)4;
    } else { /* r == 1 or r == 2 */
        factor = (seq_t)2;
    }

    return A_k * factor;
}
