/*
 * A290321  Sum modulo n of all units u in Z/nZ such that Phi(3,u) is a unit,
 *          where Phi is the cyclotomic polynomial.
 * https://oeis.org/A290321
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all u with 1 ≤ u < n, gcd(u,n)=1 and
 *            gcd(u^2+u+1,n)=1, (mod n)
 * Range:     u8 n <= 16, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* offset starts at n=2 */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t u = 1; u < n; ++u) {
        seq_t u_seq = (seq_t)u;
        /* Check u is a unit modulo n */
        if (gcd(u_seq, n_seq) != 1) continue;
        /* Compute Φ_3(u) = u^2 + u + 1 */
        seq_t phi = u_seq * u_seq + u_seq + 1;
        if (gcd(phi, n_seq) != 1) continue;
        /* Add u to the sum, staying within seq_t bounds */
        sum += u_seq;
    }
    return sum % n_seq;
}
