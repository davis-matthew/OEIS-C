/*
 * A340146  Möbius transform of A247074(x) = phi(x)/(Product_{primes p
 *          dividing x} gcd(p-1, x-1)).
 * https://oeis.org/A340146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * φ(d) / ∏_{p|d} gcd(p-1, d-1)
 * Range:     i8 n <= 260, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Validated: standard, pass, 112 terms through n = 7989 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code enumerates all divisors of n, uses the Möbius function μ,
   the Euler totient φ, a prime factorisation routine,
   and the binary GCD routine. */

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include <stdlib.h>

/* Compute Euler's totient φ of an odd 64‑bit or larger number m.
   The algorithm uses trial division up to sqrt(m). */
static seq_t calc_phi(seq_t m)
{
    seq_t result = m;
    seq_t p = 2;

    /* Handle factor 2 separately to skip it later (m is odd, but keep robust) */
    if (m % 2 == 0) {
        while (m % 2 == 0)
            m /= 2;
        result -= result / 2;
    }

    /* Trial division by odd primes */
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }

    /* If m > 1, it is a prime factor larger than sqrt(original m) */
    if (m > 1)
        result -= result / m;

    return result;
}

/* Trial‑division factorisation producing distinct primes
   pr[i] and their multiplicities ex[i].  The caller
   must provide arrays large enough for the factors. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n <= 1) { *cnt = 0; return; }
    /* Factor 2. */
    while ((n & 1) == 0) {
        if (i && pr[i - 1] == 2) {
            ++ex[i - 1];
        } else {
            pr[i] = 2;
            ex[i] = 1;
            ++i;
        }
        n >>= 1;
    }
    /* Odd factors. */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            pr[i] = p;
            ex[i] = 0;
            while (n % p == 0) {
                ++ex[i];
                n /= p;
            }
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

/* Compute A247074(d) = φ(d) / ∏_{p|d} gcd(p-1,d-1) */
static seq_t a247074_of(idx_t d)
{
    seq_t phi_val = calc_phi((seq_t)d);
    seq_t prod = 1;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(d, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        seq_t g = gcd((seq_t)(p - 1), (seq_t)(d - 1));
        prod *= g;
    }
    return phi_val / prod; /* division exact by definition */
}

/* Recursive divisor enumeration: cur holds the current divisor value */
static void divisors_rec(uint64_t *pr, uint64_t *ex, size_t len,
                         size_t idx, idx_t cur, idx_t n, seq_t *acc)
{
    if (idx == len) {
        idx_t d = cur;
        int mu = mu_of(n / d);
        seq_t val = a247074_of(d);
        *acc += (seq_t)mu * val;
        return;
    }

    uint64_t p      = pr[idx];
    uint64_t emax   = ex[idx];
    idx_t cur_val = cur;

    for (uint64_t e = 0; e <= emax; ++e) {
        divisors_rec(pr, ex, len, idx + 1, cur_val, n, acc);
        if (e < emax) {
            cur_val *= p;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 */

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    seq_t sum = 0;
    divisors_rec(pr, ex, cnt, 0, (idx_t)1, n, &sum);
    return sum;
}
