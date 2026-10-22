/*
 * A359227  Number of divisors of 4*n-3 of form 4*k+1.
 * https://oeis.org/A359227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001826(4*n-3) (= number of divisors of 4*n-3 that are ≡1
 *            (mod 4))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* undefined for n < 1, but return 1 as a safe value */

    /* Compute m = 4*n-3, avoiding overflow for reasonable n */
    __uint128_t tmp = ( (__uint128_t)4 * (uint64_t)n ) - 3;
    uint64_t m = (uint64_t)tmp;

    /* Factor m */
    uint64_t pr[64];      /* array of prime factors */
    uint64_t ex[64];      /* corresponding exponents */
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = 1;     /* multiplicative result for primes ≡1 (mod 4) */
    seq_t totalThird = 1; /* product of (e_j+1) over primes ≡3 (mod 4) */
    bool oddFlag = false; /* true if any exponent of a 3‑mod‑4 prime is odd */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        if (p == 2) {
            /* Exponent of 2 must be zero for the divisor to be ≡1 (mod 4) */
            continue;
        }

        if ((p & 3ULL) == 1ULL) {          /* p ≡ 1 (mod 4) */
            result *= (seq_t)(e + 1);
        } else {                           /* p ≡ 3 (mod 4) */
            totalThird *= (seq_t)(e + 1);
            if (e & 1ULL) oddFlag = true;
        }
    }

    /* Number of ways to pick exponents for 3‑mod‑4 primes with an even number
       of odds: (total + alt)/2, where alt = 1 if all e_j are even, else 0 */
    seq_t evenThird;
    if (oddFlag) {
        evenThird = totalThird / 2;
    } else {
        evenThird = (totalThird + 1) / 2;
    }

    result *= evenThird;
    return result;
}
