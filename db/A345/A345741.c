/*
 * A345741  a(n) = n + (n - 1) * d(n).
 * https://oeis.org/A345741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ( n if k|n else 1 )
 * Range:     u8 n <= 29, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the number of divisors of n.  The argument is an unsigned
 * 64‑bit integer; the result is returned in seq_t (which may be
 * arbitrary‑precision).  The routine is efficient enough for n up to
 * at least 10 000, using trial division. */
static seq_t divisor_count(idx_t n)
{
    seq_t d = 1;
    idx_t m = n;
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d *= (seq_t)(exp + 1);
        }
    }
    if (m > 1) {
        d *= 2;                /* remaining prime factor has exponent 1 */
    }
    return d;
}

/* Public sequence function required by the framework.  Indexes start
 * at 1 (OEIS offset).  The implementation follows the definition
 * above: a(n) = n * τ(n) + (n - τ(n)), where τ(n) is the divisor
 * count of n. */
seq_t a(idx_t n)
{
    seq_t d = divisor_count(n);
    seq_t val = (seq_t)n * d + ((seq_t)n - d);
    return val;
}
