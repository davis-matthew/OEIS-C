/*
 * A357183  Number of integer compositions of n with the same length as the
 *          absolute value of their alternating sum.
 * https://oeis.org/A357183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 22, u32 n <= 41, u64 n <= 78, u128 n <= 150,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A357183 – Number of integer compositions of n with the same length as
    the absolute value of their alternating sum.

    For a composition of length k, let
        S = y₁ – y₂ + y₃ – … (alternating sum)
    We count the compositions with |S| = k.

    The following combinatorial derivation gives the closed form:
        Let p = ceil(k/2)  → number of odd‑position parts
            q = floor(k/2) → number of even‑position parts
        Let A = sum of odd‑position parts, B = sum of even‑position parts.
        Then A + B = n and S = A – B.
        We require |S| = k, so S = ±k.

        For S = +k:
            A = (n + k)/2 ,  B = (n – k)/2
        For S = –k:
            A = (n – k)/2 ,  B = (n + k)/2

        The number of ways to distribute A among p positive integers is
            C(A-1, p-1)   (if p>0, else requires A=0).
        Analogously for B and q.

        Therefore
            a(n) = Σ_{k=1..n} Σ_{s∈{+1,-1}}
                     [ n + s*k is even ] *
                     [ A ≥ p ] * [ B ≥ q ] *
                     ways(A,p) * ways(B,q)

        where ways(A,p) = 1 if p=0 and A=0, else 0;
                        = C(A-1, p-1) if p>0 and A≥p.
*/

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* empty composition */

    seq_t res = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t p = (k + 1) / 2;           /* ceil(k/2) */
        idx_t q = k / 2;                 /* floor(k/2) */

        /* sign +1  (S = +k) */
        if ( ((n + k) & 1) == 0 )       /* n+k must be even */
        {
            idx_t A = (n + k) / 2;      /* sum of odd parts */
            idx_t B = (n - k) / 2;      /* sum of even parts */

            if (A >= p && B >= q)
            {
                seq_t ways_odd =
                    (p == 0) ? ((A == 0) ? 1 : 0) :
                    (A >= p ? binom(A - 1, p - 1) : 0);

                seq_t ways_even =
                    (q == 0) ? ((B == 0) ? 1 : 0) :
                    (B >= q ? binom(B - 1, q - 1) : 0);

                res += ways_odd * ways_even;
            }
        }

        /* sign -1  (S = -k) */
        if ( ((n + k) & 1) == 0 )       /* same parity condition */
        {
            idx_t A = (n - k) / 2;
            idx_t B = (n + k) / 2;

            if (A >= p && B >= q)
            {
                seq_t ways_odd =
                    (p == 0) ? ((A == 0) ? 1 : 0) :
                    (A >= p ? binom(A - 1, p - 1) : 0);

                seq_t ways_even =
                    (q == 0) ? ((B == 0) ? 1 : 0) :
                    (B >= q ? binom(B - 1, q - 1) : 0);

                res += ways_odd * ways_even;
            }
        }
    }

    return res;
}
