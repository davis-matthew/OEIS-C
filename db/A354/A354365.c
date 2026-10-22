/*
 * A354365  Numerators of Dirichlet inverse of primorial deflation fraction
 *          A319626(n) / A319627(n).
 * https://oeis.org/A354365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 0 if n is not squarefree; otherwise a(n) = (-1)^{ω(n)} *
 *            n / gcd(n , ∏_{p|n} prev_prime(p)) where prev_prime(p) is the
 *            largest prime < p (with prev_prime(2)=1)
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Static data for primes up to a small limit (enough for n up to about 20000). */
#define PRIME_LIMIT 20000
static bool primes_init_done = false;
static idx_t primes[PRIME_LIMIT + 1];
static int prime_count = 0;
static idx_t prev_prime[PRIME_LIMIT + 1];

static void init_primes(void)
{
    if (primes_init_done) return;
    primes_init_done = true;

    bool composite[PRIME_LIMIT + 1] = { false };
    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) {
                composite[j] = true;
            }
        }
    }
    idx_t last = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_count++] = i;
            prev_prime[i] = last;
            last = i;
        }
    }
    prev_prime[2] = 1;  /* treat prev_prime(2) as 1 for the formula */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1 */

    init_primes();

    idx_t temp = n;
    seq_t prod   = 1;          /* product of distinct primes of n  */
    seq_t denom  = 1;          /* product of previous primes    */
    int   sign   = 1;          /* (-1)^(number of distinct primes) */

    /* factorization by trial division with precomputed primes */
    for (int i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1) {
                    return 0;   /* not squarefree */
                }
            }
            prod *= (seq_t)p;
            sign = -sign;
            denom *= prev_prime[p];
        }
    }

    /* leftover prime factor > sqrt(original n) */
    if (temp > 1) {
        idx_t p = temp;
        if (p > PRIME_LIMIT) {
            /* For safety, compute previous prime by decrementing */
            idx_t cand = p - 1;
            while (cand > 2) {
                bool is_p = true;
                for (int i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= cand; ++i) {
                    if (cand % primes[i] == 0) { is_p = false; break; }
                }
                if (is_p) break;
                --cand;
            }
            if (cand > 2)
                p = cand;
            else
                p = 2;      /* fallback, should not happen for n>=2 */
        }
        prod *= (seq_t)p;
        sign = -sign;
        denom *= prev_prime[p];
    }

    seq_t abs_prod = prod;
    if (abs_prod < 0) abs_prod = -abs_prod;

    seq_t g = gcd(abs_prod, denom);
    seq_t result = (seq_t)sign * (abs_prod / g);
    return result;
}
