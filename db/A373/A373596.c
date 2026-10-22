/*
 * A373596  a(n) = 1 if n is a non-multiple of 3 whose multiplicities of prime
 *          factors of types 3m-1 and 3m+1 are both multiples of 3, otherwise
 *          0.
 * https://oeis.org/A373596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a non‑multiple of 3 whose multiplicities of
 *            prime factors of types 3m‑1 and 3m+1 are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9751, u16 n <= 9751, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* rule out multiples of 3 */
    if ((n % 3) == 0)
        return (seq_t)0;

    /* factor n; we store primes and exponents in fixed‑size arrays */
    static const size_t MAX_FACTORS = 64;       /* sufficient for 64‑bit n */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    /* factor_u64 is provided by the build environment */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned sum1 = 0; /* sum of exponents for primes ≡ 1 (mod 3) */
    unsigned sum2 = 0; /* sum of exponents for primes ≡ 2 (mod 3) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        uint64_t mod = exp % 3;         /* exponent modulo 3 */

        if (p % 3 == 1)   /* primes of type 3m+1 */
            sum1 = (sum1 + (unsigned)mod) % 3;
        else if (p % 3 == 2)  /* primes of type 3m-1 */
            sum2 = (sum2 + (unsigned)mod) % 3;
        /* primes of type 3 (i.e., p % 3 == 0) cannot appear here,
           because we already excluded multiples of 3. */
    }

    return (sum1 == 0 && sum2 == 0) ? (seq_t)1 : (seq_t)0;
}
