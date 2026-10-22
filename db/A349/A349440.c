/*
 * A349440  a(n) = n / gcd(A001608(n), n), where A001608 = Perrin sequence.
 * https://oeis.org/A349440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / gcd(P(n), n) where P is the Perrin sequence (P(0)=3,
 *            P(1)=0, P(2)=2, P(k)=P(k‑2)+P(k‑3))
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"    /* included only for seq_t & idx_t definitions */

static idx_t _gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The definition is valid for n ≥ 1; behaviour for n < 1 is irrelevant. */
    if (n < 1)
        return (seq_t)0;   /* undefined, but just in case */

    /* Special values for n = 1,2,3:  P(1)=0, P(2)=2, P(3)=3.  gcd(0,n)=n. */
    if (n <= 3)
        return (seq_t)1;   /* n / gcd(P(n), n) = 1 */

    /* Compute P(n) modulo n.  All computations are done with idx_t (uint64_t). */
    idx_t mod = n;          /* n fits in idx_t */
    idx_t p0 = 3 % mod;     /* P(0) */
    idx_t p1 = 0 % mod;     /* P(1) */
    idx_t p2 = 2 % mod;     /* P(2) */
    idx_t pk = 0;           /* placeholder for P(k) */

    /* iterate from k = 3 up to k = n */
    for (idx_t k = 3; k <= n; ++k)
    {
        pk = (p0 + p1) % mod;   /* P(k) = P(k‑2) + P(k‑3) modulo n */
        p0 = p1;
        p1 = p2;
        p2 = pk;
    }

    /* Now pk = P(n) mod n.  Compute gcd(P(n), n) using the remainder. */
    idx_t g = _gcd_uint64(pk, mod);

    /* Result fits in seq_t because a(n) ≤ n. */
    return (seq_t)(n / g);
}
