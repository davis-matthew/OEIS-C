/*
 * A331761  a(n) = Sum_{i=1..n, j=1..n, gcd(i,j)=2} (n+1-i)*(n+1-j).
 * https://oeis.org/A331761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^2 + 2*Sum_{i=2..floor(n/2)}
 *            (n+1-2*i)*(n+1-i)*phi(i)
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 578, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Handle trivial case n < 2: the loop below will be skipped */
    seq_t N = (seq_t)n;                /* cast once to seq_t for arithmetic */
    seq_t result = (N - 1) * (N - 1);  /* (n-1)^2 */
    idx_t limit = n >> 1;              /* floor(n/2) */

    for (idx_t i = 2; i <= limit; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term1 = (N + 1 - (i_seq << 1));   /* n+1-2*i  */
        seq_t term2 = (N + 1 - i_seq);          /* n+1-i    */
        seq_t phi_i = phi(i);                   /* phi(i)   */
        /* Multiply all three terms and add to result */
        result += 2 * (term1 * term2 * phi_i);
    }

    return result;
}
