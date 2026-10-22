/*
 * A337945  Numbers m with a solution (s,t,k) such that s^2 + t^2 = k*m, s + t
 *          = m, 1 <= s <= t and 1 <= k <= m - 1.
 * https://oeis.org/A337945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337945: a(n) = the nth positive integer that is NOT an odd square‑free number.
   The implementation simply scans integers upward, skipping odd square‑free values. */

#include <stdbool.h>

/* Test whether n is square‑free.  n is expected to be small for this sequence
   (in practice < 2·10⁵ for n ≤ 10⁴).  We use trial division up to sqrt(n). */
static bool is_squarefree(idx_t n)
{
    if (n < 2) return true;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            if (n % (p * p) == 0)
                return false;                /* p² divides n → not square‑free */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; the offset of the sequence is 1 (a(1)=2). */
    idx_t count = 0;      /* how many valid terms have been found */
    idx_t m = 2;          /* start candidate at 2, the first term of the sequence */

    while (true)
    {
        if ((m & 1) == 0)            /* even numbers are always in the sequence */
        {
            ++count;
            if (count == n) return (seq_t)m;
        }
        else                          /* odd m: include only if not square‑free */
        {
            if (!is_squarefree(m))
            {
                ++count;
                if (count == n) return (seq_t)m;
            }
        }
        ++m;
    }
}
