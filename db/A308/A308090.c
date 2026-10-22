/*
 * A308090  a(n) = gcd(2^n + n!, 3^n + n!, n+1).
 * https://oeis.org/A308090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2^n + n!, 3^n + n!, n+1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute modulus n+1 once, converted to seq_t. */
    idx_t mod_idx = n + 1;
    seq_t mod = (seq_t)mod_idx;

    /* factorial modulo (n+1) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact = (fact * (seq_t)i) % mod;

    /* 2^n modulo (n+1) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 = (pow2 * 2) % mod;

    /* 3^n modulo (n+1) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 = (pow3 * 3) % mod;

    seq_t a_mod = (pow2 + fact) % mod;
    seq_t b_mod = (pow3 + fact) % mod;

    seq_t g = gcd(a_mod, b_mod);
    return gcd(g, mod);
}
