/*
 * A307864  a(n) is the number of natural bases b < 2n+1 such that b^n == 1
 *          (mod 2n+1).
 * https://oeis.org/A307864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ b ∈ [1, 2n] : b^n ≡ 1 (mod 2n+1) }|
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small modular exponentiation: base^exp mod mod  */
static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t a = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (result * a) % mod;
        a = (a * a) % mod;
        exp >>= 1;
    }
    return result;
}

extern seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* a(0) = 0 by definition */

    seq_t nn = (seq_t)n;
    seq_t m  = nn * (seq_t)2 + (seq_t)1;   /* m = 2n+1 */
    seq_t count = 0;

    for (seq_t b = 1; b < m; ++b)
    {
        if (mod_pow(b, n, m) == 1)
            ++count;
    }
    return count;
}
