/*
 * A376286  n! less trailing zeros (A004154) (mod nextprime(n)).
 * https://oeis.org/A376286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = A004154(n) mod nextprime(n) (nextprime(n) is the smallest
 *            prime > n). A004154(n) = n! with all trailing zeros (factors of
 *            10) removed
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"   /* provides: uint64_t next_prime(uint64_t start); */

seq_t a(idx_t n)
{
    /* p = next prime > n */
    uint64_t p = next_prime((uint64_t)n);
    if (p <= 1) return (seq_t)0;   /* safety, though p should be prime */

    /* count exponents of 2 and 5 in n!  */
    uint64_t exp2 = 0, exp5 = 0, t;
    for (uint64_t pow = 2; pow <= (uint64_t)n; pow <<= 1) exp2 += (uint64_t)n / pow;
    for (uint64_t pow = 5; pow <= (uint64_t)n; pow *= 5) exp5 += (uint64_t)n / pow;
    t = exp5;                       /* trailing‑zero count */

    /* product modulo p, removing all 2s and 5s from each factor */
    uint64_t prod = 1 % p;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t v = (uint64_t)i;
        while ((v & 1ULL) == 0) v >>= 1;          /* strip powers of 2 */
        while (v % 5ULL == 0) v /= 5ULL;          /* strip powers of 5 */
        prod = (prod * (v % p)) % p;
    }

    /* restore the extra 2 factors (exp2 - exp5) that were over‑removed */
    uint64_t extra2 = exp2 - t;
    uint64_t pow2 = 1 % p;
    uint64_t base = 2 % p;
    while (extra2 > 0) {
        if (extra2 & 1ULL) pow2 = (pow2 * base) % p;
        base = (base * base) % p;
        extra2 >>= 1;
    }

    prod = (prod * pow2) % p;
    return (seq_t)prod;
}
