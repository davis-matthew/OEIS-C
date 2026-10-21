/*
 * A289835  Number of units u in Z/(2n-1)Z such that Phi(4,u) is a unit, where
 *          Phi is the cyclotomic polynomial.
 * https://oeis.org/A289835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, changed
 * Formula:   a(n) = ∏_{p^e‖(2n-1)} p^{e-1}·(p-3) if p≡1 (mod 4),
 *            ∏_{p^e‖(2n-1)} p^{e-1}·(p-1) if p≡3 (mod 4)
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the count of units u modulo 2n‑1 such that u²+1 is also a unit. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool _primes_inited = false;
static uint32_t _primes[20000];
static size_t _primes_cnt = 0;

static void _init_primes(void)
{
    if (_primes_inited) return;
    bool is_comp[20001] = {false};
    for (uint32_t i = 2; i * i <= 20000; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= 20000; j += i)
                is_comp[j] = true;
        }
    }
    _primes_cnt = 0;
    for (uint32_t i = 2; i <= 20000; ++i)
        if (!is_comp[i]) _primes[_primes_cnt++] = i;
    _primes_inited = true;
}

seq_t a(idx_t n)
{
    /* k = 2n-1 must be odd and ≥1 */
    uint64_t k = 2ULL * n - 1ULL;
    if (k == 1) return (seq_t)1;          /* a(1) = 1 */

    _init_primes();

    seq_t result = (seq_t)1;
    uint64_t m = k;

    for (size_t pi = 0; pi < _primes_cnt; ++pi) {
        uint32_t p = _primes[pi];
        if ((uint64_t)p * (uint64_t)p > m) break;
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            /* compute p^(e-1) */
            seq_t power = (seq_t)1;
            for (size_t i = 1; i < e; ++i) power *= (seq_t)p;

            /* pick factor depending on p mod 4 */
            seq_t factor;
            if (p % 4 == 1)
                factor = power * ((seq_t)p - (seq_t)3);
            else   /* p % 4 == 3, since p is odd prime */
                factor = power * ((seq_t)p - (seq_t)1);

            result *= factor;
        }
    }

    if (m > 1) {  /* remaining prime factor */
        uint32_t p = (uint32_t)m;          /* m fits in 32 bits here */
        seq_t factor = (seq_t)(p - (uint64_t)(p % 4 == 1 ? 3 : 1));
        result *= factor;
    }

    return result;
}
