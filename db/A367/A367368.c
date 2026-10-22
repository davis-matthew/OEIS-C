/*
 * A367368  a(n) = Sum_{(n - k) does not divide n, 0 <= k <= n} k.
 * https://oeis.org/A367368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{(n-k) ∤ n, 0 ≤ k ≤ n} k Equivalent closed form: a(n)
 *            = n*(n+1)/2 – ( n*τ(n) – σ(n) ) where τ(n) is the number of
 *            divisors and σ(n) is the sum of divisors. The implementation
 *            precomputes primes up to the required limit using a simple sieve
 *            and factors each n to obtain τ(n) and σ(n)
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- prime sieve infrastructure ------------------------------------ */
static uint32_t *primes = NULL;          /* dynamic array of primes   */
static size_t   primes_cnt = 0;          /* current count             */
static size_t   primes_cap = 0;          /* current capacity         */
static idx_t    sieve_limit = 1;         /* sieve upper bound        */

/* Ensure the prime list contains all primes ≤ limit */
static void ensure_sieve(idx_t limit) {
    if (limit <= sieve_limit) return;

    /* grow sieve_limit sufficiently */
    while (limit > sieve_limit) {
        sieve_limit = sieve_limit ? sieve_limit * 2 : 16;
    }

    /* allocate mark array */
    char *is_comp = (char *)calloc((size_t)sieve_limit + 1, 1);
    if (!is_comp) { abort(); }

    /* sieve */
    for (idx_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= sieve_limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* collect primes into dynamic array */
    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) { abort(); }
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(is_comp);
}

/* ---- main sequence function --------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* by definition, a(0)=0 */

    /* total sum 0..n */
    seq_t n_seq = (seq_t)n;
    seq_t total = n_seq * (n_seq + 1) / 2;

    /* factor n to obtain τ(n) and σ(n) */
    idx_t tmp = n;
    seq_t tau   = 1;          /* number of divisors */
    seq_t sigma = 1;          /* sum of divisors   */
    ensure_sieve(tmp);        /* primes up to tmp  */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        if (tmp % p != 0) continue;
        idx_t e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }

        tau *= (seq_t)(e + 1);

        /* compute (p^(e+1) - 1) / (p - 1) */
        seq_t p_pow = 1;
        for (idx_t j = 0; j < e + 1; ++j) p_pow *= (seq_t)p;
        seq_t term = (p_pow - 1) / (p - 1);
        sigma *= term;
    }
    if (tmp > 1) {                  /* remaining prime factor */
        tau   *= 2;
        sigma *= (seq_t)(tmp + 1);
    }

    /* a(n) = total - (n*tau - sigma) */
    seq_t subtract = n_seq * tau - sigma;
    seq_t result = total - subtract;
    return result;
}
