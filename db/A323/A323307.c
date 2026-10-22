/*
 * A323307  Number of ways to fill a matrix with the parts of a multiset whose
 *          multiplicities are the prime indices of n.
 * https://oeis.org/A323307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A318762(n) * A000005(A056239(n)) where - A056239(n) =
 *            Σ_{p^e||n} e ⋅ ind(p) - A318762(n) = ( A056239(n) )! /
 *            ∏_{p^e||n} (ind(p)! )^e (ind(p) is the index of prime p in the
 *            prime sequence, starting with 2 having index 1) This function
 *            computes a(n) directly
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- small helpers ---------- */

static seq_t factorial_upto(idx_t m) {
    seq_t f = 1;
    for (idx_t i = 1; i <= m; ++i) f *= (seq_t)i;
    return f;
}

static seq_t divisor_count(idx_t n) {
    seq_t d = 1;
    idx_t t = n;
    for (idx_t i = 2; i * i <= t; ++i) {
        if (t % i == 0) {
            idx_t c = 0;
            while (t % i == 0) {
                t /= i;
                ++c;
            }
            d *= (c + 1);
        }
    }
    if (t > 1) d *= 2;
    return d;
}

/* ---------- prime sieve (static) ---------- */

static idx_t prime_limit = 0;
static idx_t *primes = NULL;          /* array of primes up to prime_limit   */
static idx_t prime_count = 0;         /* number of primes in the array       */

static void build_prime_sieve(idx_t limit) {
    if (limit <= prime_limit) return;

    if (primes) { free(primes); primes = NULL; }
    prime_limit = limit;
    char *is_prime = (char*)malloc((limit + 1) * sizeof(char));
    memset(is_prime, 1, (limit + 1) * sizeof(char));
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    /* count primes */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++prime_count;

    primes = (idx_t*)malloc(prime_count * sizeof(idx_t));
    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
}

/* binary search for prime index (1‑based) */
static idx_t prime_index(idx_t p) {
    idx_t lo = 0, hi = prime_count;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo + 1;  /* indices start at 1 */
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;  /* a(1) = 1 */

    /* build sieve up to n for factorisation and prime indices */
    build_prime_sieve(n);

    /* factorisation of n */
    /* temporary storage for factors (size never exceeds prime_count) */
    idx_t *fac_p = (idx_t*)malloc(prime_count * sizeof(idx_t));
    idx_t *fac_e = (idx_t*)malloc(prime_count * sizeof(idx_t));
    idx_t fac_len = 0;

    idx_t temp = n;
    for (idx_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac_p[fac_len] = p;
            fac_e[fac_len] = e;
            ++fac_len;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        fac_p[fac_len] = temp;
        fac_e[fac_len] = 1;
        ++fac_len;
    }

    /* compute S = Σ e * ind(p) and denominator product of (ind(p)! )^e */
    seq_t S = 0;
    seq_t denomProd = 1;
    for (idx_t i = 0; i < fac_len; ++i) {
        idx_t p = fac_p[i];
        idx_t e = fac_e[i];
        idx_t idx = prime_index(p);          /* 1‑based index of p */

        S += (seq_t)idx * e;

        seq_t factorial_of_idx = factorial_upto(idx);
        for (idx_t rep = 0; rep < e; ++rep) denomProd *= factorial_of_idx;
    }

    /* T = S! / denomProd */
    seq_t factS = factorial_upto((idx_t)S);
    seq_t T = factS / denomProd;

    /* dS = divisor count of S */
    seq_t dS = divisor_count((idx_t)S);

    free(fac_p);
    free(fac_e);

    return T * dS;
}
