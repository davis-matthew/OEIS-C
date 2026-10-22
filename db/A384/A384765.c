/*
 * A384765  a(n) is the rad(n)-th cyclotomic polynomial evaluated at x = -1.
 * https://oeis.org/A384765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Φ_{rad(n)}(-1) where Φ_m is the m‑th cyclotomic
 *            polynomial
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For squarefree m: Φ_m(-1) = 1 if m>1 is odd, Φ_m(-1) = 0 if m=2,                */
/* Φ_{2p}(-1) = p for odd prime p, and Φ_{2k}(-1) = 1 when k is odd composite.      */
/* For m=1 we have Φ_1(x)=x-1, so Φ_1(-1) = -2. */

#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = -2 */
        return (seq_t)-2;

    /* compute radial value rad(n) = product of distinct primes dividing n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= primes[i];

    if (rad == 1)             /* only when n == 1, handled above */
        return (seq_t)-2;

    if ((rad & 1ULL) == 0) {  /* rad is even  => rad = 2 * k, k odd */
        uint64_t k = rad >> 1;        /* odd part */
        if (k == 1)                 /* rad == 2  -> a = 0 */
            return (seq_t)0;
        if (is_prime((idx_t)k))     /* rad = 2 * prime */
            return (seq_t)k;
        /* rad = 2 * (odd composite) */
        return (seq_t)1;
    } else {                     /* rad is odd (>1) */
        return (seq_t)1;
    }
}
