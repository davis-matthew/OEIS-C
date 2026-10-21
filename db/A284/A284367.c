/*
 * A284367  Square array T(n,k) = number of separable polynomials of degree <=
 *          k in Z/n[x], n>=1, k>=1, read by antidiagonals.
 * https://oeis.org/A284367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the row and column of the n‑th
 *            entry of the square array T, read by antidiagonals. T(n,k) =
 *            product over prime powers p^e | n of p^((k+1)(e-1)) * T(p,k)
 *            T(p,k) = (p-1) + (p-1)p + (p-1)^2 * Σ_{d=2..k}p^{d-1} (for k≥1).
 *            This matches the definition of A284367
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   T_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/T_prime.h"

/* ------------------------------------------------------------------ */
/* Multiply a base of type seq_t by itself exp times (exp is idx_t). */
static seq_t pow_u64(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= base;
    return res;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* T for a prime power p^e and exponent k (degree limit). */
static seq_t T_prime_power(uint64_t p, idx_t e, idx_t k)
{
    if (e == 1) return T_prime(p, k);

    /* factor = p^((k+1)*(e-1)) */
    idx_t factor_exp = (k + 1) * (e - 1);          /* exponent fits in idx_t */
    seq_t factor = pow_u64((seq_t)p, factor_exp);

    seq_t base = T_prime(p, k);
    return factor * base;
}

/* ------------------------------------------------------------------ */
/* Factor n into primes and exponents.  primes[16] and exps[16] are
   assumed large enough for the expected n (≤ 10 000).  pcnt is set to
   the number of distinct primes. */
static void factor_uint64(uint64_t n, uint64_t primes[16], idx_t exps[16],
                          idx_t *pcnt)
{
    idx_t cnt = 0, e;

    /* factor 2 */
    e = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++e;
    }
    if (e) {
        primes[cnt] = 2ULL;
        exps[cnt] = e;
        ++cnt;
    }

    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        if (e) {
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }

    if (n > 1) {
        primes[cnt] = n;
        exps[cnt] = 1;
        ++cnt;
    }

    *pcnt = cnt;
}

/* ------------------------------------------------------------------ */
/* Main entry function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds guard */

    /* Determine (i,j) from the antidiagonal position n. */
    idx_t s = 1;
    /* find smallest s with s*(s+1)/2 ≥ n */
    while (s * (s + 1) / 2 < n) ++s;

    idx_t prev = (s - 1) * s / 2;
    idx_t offset = n - prev;        /* 1‑based position on diagonal */
    idx_t i = offset;               /* row index (n‑parameter) */
    idx_t j = s - offset + 1;       /* column index (k‑parameter) */

    /* T(1,⋅) = 1 for all k. */
    if (i == 1) return 1;

    /* Compute T(i,j) by prime factorisation. */
    uint64_t primes[16];
    idx_t exps[16];
    idx_t pcnt;
    factor_uint64((uint64_t)i, primes, exps, &pcnt);

    seq_t result = 1;
    for (idx_t idx = 0; idx < pcnt; ++idx) {
        seq_t part = T_prime_power(primes[idx], exps[idx], j);
        result *= part;
    }
    return result;
}
