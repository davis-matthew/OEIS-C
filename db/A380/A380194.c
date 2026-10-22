/*
 * A380194  Continued fraction expansion of Sum_{i>=0} (-1)^i/(q(i)*q(i+1))
 *          where q(0)=q(1)=1, q(3n+2)=q(3n+1)+q(3n), q(3n+3)=q(3n+2)+q(3n+1),
 *          and q(3n+4)=q(3n+2)*(q(3n+2)*q(3n+3)+1).
 * https://oeis.org/A380194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 0; a(n) = 1 if n % 3 == 0 or n % 3 == 2; a(n) = q(n-2)^2
 *            for n >= 4 and n % 3 == 1; where q is defined by: q(0) = q(1) =
 *            1; for i >= 2: if i % 3 == 1 and i >= 4: q(i) = q(i-2) * (q(i-2)
 *            * q(i-1) + 1); else q(i) = q(i-1) + q(i-2);
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   compute_q
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_q.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t mod = n % 3;
    if (mod == 1) {
        if (n < 4) return (seq_t)1;  /* a(1) = 1 */
        idx_t m = n - 2;
        seq_t qm = compute_q(m);
        return qm * qm;
    }
    return (seq_t)1;
}
