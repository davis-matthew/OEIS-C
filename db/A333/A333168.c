/*
 * A333168  a(n) = Sum_{k=0..n} r_2(k^2 + 1), where r_2(k) is the number of
 *          ways of writing k as a sum of 2 squares (A004018).
 * https://oeis.org/A333168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^n r_2(k^2 + 1)
 * Range:     u8 n <= 22, u16 n <= 2862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9878 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute r_2(m): number of representations of m as a sum of two squares.
   Uses the theorem that r_2(m) = 4 * product_{p ≡ 1 (mod 4)} (e_p + 1)
   if every prime ≡ 3 (mod 4) divides m to an even power; otherwise r_2(m)=0.
   All intermediate calculations fit in uint64_t for the ranges considered. */
static seq_t r2_uint64(uint64_t m)
{
    if (m == 1)        /* A004018(1) = 4 */
        return (seq_t)4;

    uint64_t prod = 1;          /* product of (e+1) for primes ≡1 mod 4 */
    uint64_t x = m;

    for (uint64_t d = 2; d * d <= x; ++d)
    {
        if (x % d != 0)
            continue;

        uint64_t cnt = 0;
        while (x % d == 0) {
            x /= d;
            ++cnt;
        }

        if (d == 2)            /* prime 2 does not affect the product */
            continue;

        if (d % 4 == 3) {      /* odd exponent of a 3 (mod 4) prime → impossible */
            if (cnt & 1)
                return (seq_t)0;
        } else if (d % 4 == 1)  /* prime ≡1 (mod 4) contributes (e+1) */
            prod *= (uint64_t)(cnt + 1);
    }

    /* Remaining prime factor, if any */
    if (x > 1) {
        if (x % 4 == 3)          /* leftover prime ≡3 (mod 4) */
            return (seq_t)0;
        if (x % 4 == 1)          /* one more prime ≡1 (mod 4) with exponent 1 */
            prod *= 2;
    }

    return (seq_t)(4 * prod);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        uint64_t m = (uint64_t)k * (uint64_t)k + 1ULL;
        sum += r2_uint64(m);
    }
    return sum;
}
