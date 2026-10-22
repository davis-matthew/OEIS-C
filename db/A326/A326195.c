/*
 * A326195  Number of iterations of x -> A009195(x) needed to reach 1 when
 *          starting from x = n, where A009195(x) = gcd(x, phi(x)).
 * https://oeis.org/A326195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 (n=1) else 1 + a(gcd(n,phi(n)))
 * Range:     u8 n <= 4643, u16 n <= 9990, u32 n <= 9990, u64 n <= 9990,
 *            u128 n <= 9990, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided gcd and phi helpers. */

#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t steps = 0;
    seq_t cur = (seq_t)n;

    while (cur != 1)
    {
        seq_t phi_val = phi((idx_t)cur);
        cur = gcd(cur, phi_val);
        ++steps;
    }

    return steps;
}
