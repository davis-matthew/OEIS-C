/*
 * A273319  a(n) = ((2*n+1)^(n+1) + (-1)^n)/(n+1)^2.
 * https://oeis.org/A273319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n+1)^(n+1) + (-1)^n) / (n+1)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For small n up to 13 the result fits in 64 bits.
       Compute using unsigned __int128 to avoid overflow. */
    if (n <= 13) {
#if defined(__SIZEOF_INT128__)   /* compiler supports 128‑bit integers */
        __uint128_t base = (unsigned __int128)(2 * n + 1);
        __uint128_t res  = 1;
        idx_t       exp  = n + 1;

        while (exp) {
            if (exp & 1U)
                res *= base;
            base *= base;
            exp >>= 1U;
        }

        /* add the +/- 1 from (-1)^n */
        if (n & 1U)
            res -= 1;
        else
            res += 1;

        /* divide by (n+1)^2 */
        __uint128_t den = (unsigned __int128)(n + 1);
        den *= den;

        return (seq_t)(res / den);
#else
        /* Fallback: use big_pow even for small n. */
        seq_t base = (seq_t)(2 * n + 1);
        seq_t num  = big_pow(base, n + 1);
        if (n & 1U)
            num -= 1;
        else
            num += 1;
        seq_t den = (seq_t)(n + 1);
        den *= den;
        return num / den;
#endif
    }

    /* For larger n use the generic big_pow; the result may be huge
       but is required only when seq_t is an arbitrary‑precision type. */
    seq_t base = (seq_t)(2 * n + 1);
    seq_t num  = big_pow(base, n + 1);

    if (n & 1U)
        num -= 1;
    else
        num += 1;

    seq_t den = (seq_t)(n + 1);
    den *= den;

    return num / den;
}
