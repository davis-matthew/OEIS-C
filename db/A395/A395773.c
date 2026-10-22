/*
 * A395773  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-15,.) mod 15.
 * https://oeis.org/A395773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(3^e) = 3^e, a(5^e) = 5^e, a(p^e) =
 *            p^(e‑1)*(p‑1) if p ≡ 1,2,4,8 (mod 15), a(p^e) = p^(e‑1)*(p+1) if
 *            p ≡ 7,11,13,14 (mod 15)
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;

    uint64_t temp = (uint64_t)n;
    uint64_t p = 2;
    while (p <= temp / p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }

            seq_t term;
            if (p == 3 || p == 5) {
                term = 1;
                for (uint64_t k = 0; k < exp; ++k)
                    term *= (seq_t)p;
            } else {
                seq_t p_pow = 1;
                for (uint64_t k = 0; k + 1 < exp; ++k)
                    p_pow *= (seq_t)p;

                bool use_minus = (p % 15 == 1) || (p % 15 == 2) ||
                                 (p % 15 == 4) || (p % 15 == 8);
                if (use_minus)
                    term = p_pow * (seq_t)(p - 1);
                else
                    term = p_pow * (seq_t)(p + 1);
            }
            result *= term;
        }
        p++;
    }

    if (temp > 1) {
        uint64_t p2 = temp;
        seq_t term;
        if (p2 == 3 || p2 == 5) {
            term = (seq_t)p2;
        } else {
            bool use_minus = (p2 % 15 == 1) || (p2 % 15 == 2) ||
                             (p2 % 15 == 4) || (p2 % 15 == 8);
            if (use_minus)
                term = (seq_t)(p2 - 1);
            else
                term = (seq_t)(p2 + 1);
        }
        result *= term;
    }

    return result;
}
