/*
 * A372761  Denominator of the continued fraction
 *          1/(2-3/(3-4/(4-5/(...(n-1)-n/(n+4))))).
 * https://oeis.org/A372761
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A372761 – Denominator of the continued fraction
 *
 *  a(n) = (5*n-4)/gcd(5*n-4, A051403(n-2)+4*A051403(n-3))
 *  The implementation below uses the fact that
 *  p2 = 5*n-4 and that gcd(p2, p3) gives the reduction factor,
 *  where p3 is obtained from the continued‑fraction recurrence.
 *  By computing all intermediates modulo p2 we obtain the necessary
 *  remainder without dealing with gigantic integers.
 */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 3.  For smaller n return 0. */
    if (n < 3) return 0;

    /* p2 = 5*n-4 – the reduced denominator before cancellation. */
    seq_t p2 = 5 * (seq_t)n - 4;
    idx_t p2_int = (idx_t)p2;               /* p2 fits in idx_t for n <= ~1e4 */

    /* Base of the backward recurrence (k = n-1).  All values are taken
     * modulo p2 to keep them small. */
    seq_t p = ((seq_t)n * (seq_t)n + 2 * (seq_t)n - 4) % p2;
    seq_t q = ((seq_t)n + 4) % p2;

    /* Run the recurrence downwards from k = n-2 to k = 2
     * (inclusive).  After the last iteration q holds p3 mod p2. */
    for (idx_t k = n - 2; k >= 2; --k) {
        seq_t x = ((seq_t)k * p) % p2;
        seq_t y = ((seq_t)(k + 1) * q) % p2;
        seq_t np = (x + p2 - y) % p2;  /* k*p - (k+1)*q  (mod p2)   */
        seq_t nq = p;                  /* q_next = p_old    */
        p = np;
        q = nq;
    }

    /* q now equals p3 modulo p2.  The reduction factor is gcd(p2, q). */
    seq_t g = gcd(p2, q);
    return p2 / g;
}
