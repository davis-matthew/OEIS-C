/*
 * A336835  Number of iterations of x -> A003961(x) needed before the result
 *          is deficient (sigma(x) < 2x), when starting from x=n.
 * https://oeis.org/A336835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of the prime‑shift function A003961
 *            until the result becomes deficient (σ(x) < 2x), starting from x
 *            = n
 * Range:     u8 n <= 23, u16 n <= 6737, u32 n <= 8961, u64 n <= 8961,
 *            u128 n <= 8961, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External type definitions.
    Do NOT define your own seq_t or idx_t – they are supplied by the build. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

/* ------------------------------------------------------------------
   Helper: factorization of a 64‑bit integer.
   The function stores distinct prime factors in 'pr' and their
   exponents in 'ex'.  The number of factors is written to '*cnt'.
   ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n,
                          uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;

    while ((n & 1ULL) == 0ULL) {           /* factor 2 */
        pr[(*cnt)] = 2ULL;
        ex[(*cnt)] = 0ULL;
        do { n >>= 1ULL; ex[(*cnt)]++; } while ((n & 1ULL) == 0ULL);
        (*cnt)++;
    }
    uint64_t p = 3ULL;
    while (p * p <= n) {
        if (n % p == 0ULL) {
            pr[(*cnt)] = p;
            ex[(*cnt)] = 0ULL;
            do { n /= p; ex[(*cnt)]++; } while (n % p == 0ULL);
            (*cnt)++;
        }
        p += 2ULL;
    }
    if (n > 1ULL) {                       /* remaining prime */
        pr[(*cnt)] = n;
        ex[(*cnt)] = 1ULL;
        (*cnt)++;
    }
}

/* ------------------------------------------------------------------
   Helper: compute σ(n) from its prime factorization.
   Uses the multiplicative formula:
        σ(n) = ∏ (p^(e+1) - 1) / (p - 1)
   ------------------------------------------------------------------ */
static seq_t sigma_from_factors(const uint64_t *pr,
                                const uint64_t *ex, size_t cnt)
{
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        uint64_t e = ex[i];

        seq_t pow = 1;
        /* compute p^(e+1) */
        for (uint64_t k = 0; k < e + 1; ++k) pow *= p;

        seq_t term = (pow - 1) / (p - 1);
        result *= term;
    }
    return result;
}

/* ------------------------------------------------------------------
   Helper: next prime after a given prime 'p'.
   ------------------------------------------------------------------ */
static uint64_t next_prime_uint64(uint64_t p)
{
    /* p is guaranteed to be a prime.  Search for next prime > p. */
    uint64_t q = p + 1ULL;
    while (!is_prime((idx_t)q)) q++;
    return q;
}

/* ------------------------------------------------------------------
   Helper: prime‑shift of a 64‑bit integer.
   For each prime factor p^e replace p by the next prime q^e.
   ------------------------------------------------------------------ */
static uint64_t prime_shift_uint64(uint64_t n)
{
    if (n < 2) return 1;                 /* shift of 1 is 1 */

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64(n, pr, ex, &cnt);

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime_uint64(pr[i]);   /* next prime after p */
        seq_t term = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) term *= (seq_t)q;
        result *= (uint64_t)term;
    }
    return result;
}

/* ------------------------------------------------------------------
   Helper: σ(n) for a 64‑bit integer.
   ------------------------------------------------------------------ */
static seq_t sigma_of_uint64(uint64_t n)
{
    if (n < 2) return 1;                 /* σ(1) = 1 */

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_uint64(n, pr, ex, &cnt);
    return sigma_from_factors(pr, ex, cnt);
}

/* ------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset handling */

    uint64_t x = (uint64_t)n;
    seq_t count = 0;

    for (;;) {
        seq_t sigma = sigma_of_uint64(x);
        seq_t twice = (seq_t)2 * (seq_t)x;

        if (sigma < twice) break;        /* x becomes deficient */
        x = prime_shift_uint64(x);       /* next iterate */
        ++count;
    }
    return count;
}
