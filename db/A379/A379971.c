/*
 * A379971  a(n) = 1 if n^2-1 is squarefree, otherwise 0.
 * https://oeis.org/A379971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n^2-1 is squarefree, otherwise 0 (OEIS A379971)
 * Range:     u8 n <= 9250, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool squarefree_uint64(uint64_t m)
{
    if (m <= 1) return true;          /* 0 and 1 are square‑free by convention */
    /* check for factor 2^2 = 4 */
    if ((m & 3) == 0) return false;   /* m % 4 == 0 → divisible by 4 */
    /* test odd primes up to sqrt(m) by trying all odd numbers */
    for (uint64_t p = 3; p <= m / p; p += 2)
    {
        uint64_t p2 = p * p;          /* p ≤ sqrt(m) guarantees no overflow */
        if (m % p2 == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 0 */
    if (n <= 1) return 0;

    /* n odd ⇒ n^2−1 is always divisible by 4 ⇒ not square‑free */
    if ((n & 1ULL) != 0) return 0;

    /* For even n, n^2−1 = (n−1)(n+1) with coprime odd factors.
       It is square‑free iff both factors are square‑free. */
    uint64_t m1 = n - 1;
    uint64_t m2 = n + 1;

    if (!squarefree_uint64(m1) || !squarefree_uint64(m2))
        return 0;

    return 1;
}
