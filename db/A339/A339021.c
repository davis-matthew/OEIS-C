/*
 * A339021  Number of pairs of divisors of 2n, (d1,d2), such that d1 < d2 and
 *          at least one of d1*d2 +- 1 is prime.
 * https://oeis.org/A339021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of divisors (d1,d2) of 2n with d1 < d2
 *            such that at least one of d1*d2 ± 1 is prime
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- helper: primality test for 64‑bit unsigned integers ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5, w = 2;
    while (i <= n / i) {
        if (n % i == 0) return false;
        i += w;
        w = 6 - w;
    }
    return true;
}

/* ---------- helper: factorization of a 64‑bit integer into primes ---------- */
static void factor_uint64(uint64_t n, uint64_t primes[], uint64_t exps[], size_t *cnt)
{
    size_t c = 0;
    if (n % 2 == 0) {
        uint64_t e = 0;
        while ((n % 2) == 0) { n /= 2; ++e; }
        primes[c] = 2; exps[c] = e; ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[c] = p; exps[c] = e; ++c;
        }
    }
    if (n > 1) { primes[c] = n; exps[c] = 1; ++c; }
    *cnt = c;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* m = 2*n ; use seq_t for safety, but 64‑bit is enough for requested range */
    seq_t m_seq = ((seq_t)n) << 1;
    uint64_t m = (uint64_t)m_seq;        /* 2*n fits in 64 bits for the intended ranges */

    /* factorize m */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t numf = 0;
    factor_uint64(m, primes, exps, &numf);

    /* compute total number of divisors of m */
    size_t divCnt = 1;
    for (size_t i = 0; i < numf; ++i) divCnt *= (size_t)(exps[i] + 1);

    /* generate all divisors of m */
    uint64_t *divs = (uint64_t *)malloc(divCnt * sizeof(uint64_t));
    if (!divs) return (seq_t)0;   /* allocation failure – unlikely in this context */
    size_t cur = 1;
    divs[0] = 1U;
    for (size_t i = 0; i < numf; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t old = cur;
        uint64_t mult = 1;
        for (size_t k = 0; k < e; ++k) {
            mult *= p;
            for (size_t j = 0; j < old; ++j) {
                divs[cur++] = divs[j] * mult;
            }
        }
    }

    /* count qualifying divisor pairs */
    seq_t count = 0;
    for (size_t i = 0; i < divCnt; ++i) {
        for (size_t j = i + 1; j < divCnt; ++j) {
            uint64_t prod = divs[i] * divs[j];
            if (is_prime_uint64(prod + 1) || is_prime_uint64(prod - 1))
                ++count;
        }
    }

    free(divs);
    return count;
}
