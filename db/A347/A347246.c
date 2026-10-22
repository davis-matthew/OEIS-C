/*
 * A347246  a(n) = 1 if the greatest prime factor of A000593(n) [sum of odd
 *          divisors of n] is at least as large as the greatest prime factor
 *          of n itself, otherwise a(n) = 0.
 * https://oeis.org/A347246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the greatest prime factor of A000593(n) ( = sum of
 *            odd divisors of n ) is at least as large as the greatest prime
 *            factor of n, else 0
 * Range:     u8 n <= 9355, u16 n <= 9355, u32 n <= 9355, u64 n <= 9355,
 *            u128 n <= 9355, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000593(n) is multiplicative; for n=∏p_i^{e_i} (p_i odd)
   it equals ∏ (p_i^{e_i+1}-1)/(p_i-1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/lpf.h"      /* idx_t lpf(idx_t n); */

static uint64_t max_prime_factor(uint64_t x)
{
    uint64_t max = 0;
    while (x > 1U) {
        uint64_t p = (uint64_t)lpf(x);      /* smallest prime factor */
        if (p <= 1U) break;                 /* safety */
        if (p > max) max = p;
        while (x % p == 0U) x /= p;
    }
    return max;
}

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)1;

    /* Greatest prime factor of n. */
    uint64_t gpf_n = max_prime_factor((uint64_t)n);

    /* Sum of odd divisors (A000593). */
    uint64_t odd = n;
    while ((odd & 1ULL) == 0ULL) odd >>= 1U;   /* remove all factors of 2 */

    uint64_t temp = odd;
    unsigned __int128 sum128 = 1U;            /* product of terms */

    while (temp > 1U) {
        uint64_t p = (uint64_t)lpf(temp);     /* odd prime factor */
        unsigned exp = 0U;
        while (temp % p == 0U) {
            temp /= p;
            exp++;
        }

        /* Compute (p^(exp+1)-1)/(p-1) as 128‑bit integer. */
        unsigned __int128 powp = 1U;
        for (unsigned i = 0; i < exp + 1U; ++i) powp *= (unsigned __int128)p;
        unsigned __int128 term = (powp - 1U) / (unsigned __int128)(p - 1U);

        sum128 *= term;
    }

    /* The sum definitely fits within 64‑bit for tested n. */
    uint64_t sum = (uint64_t)sum128;

    uint64_t gpf_sum = max_prime_factor(sum);

    return (seq_t)(gpf_sum >= gpf_n ? 1 : 0);
}
