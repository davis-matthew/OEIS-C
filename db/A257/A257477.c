/*
 * A257477  Multiplicative with a(2) = 0, a(4) = -1, a(2^e) = 0 if e>2, a(3) =
 *          -1, a(3^e) = 0^e if e>1, a(p^e) = 1 if p == 1, 3 (mod 8), a(p^e) =
 *          (-1)^e if p == 5, 7 (mod 8).
 * https://oeis.org/A257477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative; a(1)=1; a(2)=0, a(4)=-1, a(2^e)=0 for
 *            e>2; a(3)=-1, a(3^e)=0 for e>1; for other primes p, a(p^e)=1 if
 *            p≡1 or 3 (mod 8), a(p^e)=(-1)^e if p≡5 or 7 (mod 8)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* factor_n may need at least 16 slots for 64‑bit n */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 2)
        {
            if (e == 1)          val = 0;
            else if (e == 2)     val = -1;
            else                 val = 0;
        }
        else if (p == 3)
        {
            if (e == 1)          val = -1;
            else                 val = 0;
        }
        else
        {
            uint64_t mod = p & 7;                /* p % 8 */
            if (mod == 1 || mod == 3)            val = 1;
            else                                 val = (e & 1) ? -1 : 1;
        }

        result *= val;
        if (result == 0) break;                  /* product stays 0 */
    }

    return result;
}
