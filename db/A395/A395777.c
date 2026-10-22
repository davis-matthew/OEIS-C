/*
 * A395777  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-24,.) mod 24.
 * https://oeis.org/A395777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 246, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A395777 – twisted Euler phi-function for the Kronecker symbol (-24,·) mod 24.
 *
 *  Formula:
 *      a(1) = 1
 *      a is multiplicative and for a prime power p^e | n
 *          a(2^e) = 2^e
 *          a(3^e) = 3^e
 *          if p ≡ 1,5,7,11 (mod 24)   a(p^e) = p^{e-1}·(p−1)
 *          if p ≡ 13,17,19,23 (mod 24)  a(p^e) = p^{e-1}·(p+1)
 *
 *  This implementation factors n by trial division and multiplies the
 *  local contributions using the helper routine big_pow from the utils
 *  library.  It works for any seq_t/idx_t pair supplied by the build
 *  system and requires no external state.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t
_factor_prime(uint64_t p, int e)
{
    /* 2 and 3: direct powers */
    if (p == 2ull || p == 3ull)
        return big_pow((seq_t)p, (seq_t)e);

    /* General prime */
    seq_t val = big_pow((seq_t)p, (seq_t)(e - 1));
    uint64_t mod = p % 24ull;

    /* Residue classes decide whether to use (p‑1) or (p+1). */
    if (mod == 1ull || mod == 5ull || mod == 7ull || mod == 11ull)
        val *= (seq_t)(p - 1ull);
    else               /* mod == 13,17,19,23 (p is odd, ≠3) */
        val *= (seq_t)(p + 1ull);

    return val;
}

seq_t
a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* value below offset is irrelevant */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2ull; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            result *= _factor_prime(p, e);
        }
    }

    if (m > 1ull)          /* remaining prime factor */
        result *= _factor_prime(m, 1);

    return result;
}
