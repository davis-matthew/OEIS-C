/*
 * A258817  a(n) = (!0 + !1 +... + !(n-1)) mod n.
 * https://oeis.org/A258817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (!0 + !1 + ... + !(n-1)) mod n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* guard, not part of the sequence */
    seq_t mod = (seq_t)n;

    seq_t d_prev2 = ((seq_t)1) % mod;  /* !0 */
    seq_t d_prev1 = ((seq_t)0) % mod;  /* !1 */
    seq_t sum    = ((seq_t)0) % mod;

    /* add !0 */
    sum = (sum + d_prev2) % mod;

    /* add !1 if needed */
    if (n > 1) {
        sum = (sum + d_prev1) % mod;
    }

    for (idx_t k = 2; k < n; ++k) {
        seq_t d_curr = ((seq_t)(k - 1) * (d_prev1 + d_prev2)) % mod;
        sum = (sum + d_curr) % mod;

        d_prev2 = d_prev1;
        d_prev1 = d_curr;
    }

    return sum;
}
