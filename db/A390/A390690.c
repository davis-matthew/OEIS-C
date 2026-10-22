/*
 * A390690  The sum of the divisors of n that are Loeschian numbers (A003136).
 * https://oeis.org/A390690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^e‖n} (p^(e+1)-1)/(p-1) if p≠2 (mod 3); a(n) =
 *            Π_{p^e‖n} (p^(2*(⌊e/2⌋+1))-1)/(p²-1) if p≡2 (mod 3)
 * Range:     u8 n <= 18, u16 n <= 8226, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
# include "../utils/big_pow.h"
# include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard anyway */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term, numerator, denom;

        if (p % 3 == 2) {                  /* p ≡ 2 (mod 3) */
            uint64_t h = e / 2 + 1;        /* floor(e/2)+1 */
            numerator = big_pow((seq_t)p,
                                (seq_t)(2 * h));
            numerator -= 1;
            denom = ((seq_t)p * (seq_t)p) - 1;
        } else {                            /* p = 3 or p ≡ 1 (mod 3) */
            numerator = big_pow((seq_t)p, (seq_t)(e + 1));
            numerator -= 1;
            denom = (seq_t)p - 1;
        }
        term = numerator / denom;          /* exact division */
        result *= term;
    }

    return result;
}
