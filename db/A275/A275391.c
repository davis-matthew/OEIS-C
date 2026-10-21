/*
 * A275391  Least k such that n divides sigma(k^k) (k > 0).
 * https://oeis.org/A275391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least k>0 such that n divides σ(k^k)
   (OEIS A275391).  The value is returned as seq_t.
   This implementation searches k upward and uses the
   multiplicative formula of σ in modular arithmetic. */
#include <stdint.h>
#include <stdbool.h>

/* The mod_pow routine is provided by the Seq­ium library. */
#include "../utils/mod_pow.h"

#define MAX_PRIME 20000

/* Prime list - generated once at first use. */
static bool primes_init = false;
static size_t prime_cnt = 0;
static uint64_t primes[MAX_PRIME + 1];

/* One-time sieve to fill the primes array. */
static void init_primes(void)
{
    if (primes_init) return;
    bool composite[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i) composite[i] = false;
    for (uint64_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!composite[i]) {
            for (uint64_t j = i * i; j <= MAX_PRIME; j += i)
                composite[j] = true;
        }
    }
    for (uint64_t i = 2; i <= MAX_PRIME; ++i)
        if (!composite[i]) primes[prime_cnt++] = i;
    primes_init = true;
}

/* Compute σ(k^k) modulo mod (mod fits in seq_t). */
static seq_t sigma_kpk_mod(uint64_t k, uint64_t mod)
{
    init_primes();

    seq_t result = 1;           /* product of factor sums modulo mod */
    seq_t denom  = (seq_t)mod;  /* keep mod as seq_t for later use */

    uint64_t tmp = k;
    size_t prime_index = 0;

    /* Factor k */
    while (prime_index < prime_cnt && primes[prime_index] * primes[prime_index] <= tmp) {
        uint64_t p = primes[prime_index];
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp > 0) {
            /* E = exp * k */
            uint64_t E = exp * k;

            /* Compute sum_{j=0}^E p^j mod mod */
            /* sum = (p^{E+1} - 1)/(p-1)  (exact division) */
            seq_t p_seq    = (seq_t)p;
            seq_t modmp    = denom * (seq_t)(p - 1);        /* mod * (p-1) */
            seq_t pow_val  = mod_pow(p_seq, E + 1, modmp);
            seq_t numerator = pow_val + modmp - 1;         /* = p^{E+1} - 1 (mod modmp) */
            seq_t sum = numerator / (seq_t)(p - 1);        /* exact division */
            sum = sum % denom;                            /* reduce modulo mod */

            result = (result * sum) % denom;
        }
        ++prime_index;
    }

    if (tmp > 1) {  /* remaining prime factor */
        uint64_t p = tmp;          /* exponent exp = 1 */
        uint64_t E = k;            /* e*k = 1*k */

        seq_t p_seq    = (seq_t)p;
        seq_t modmp    = denom * (seq_t)(p - 1);        /* mod * (p-1) */
        seq_t pow_val  = mod_pow(p_seq, E + 1, modmp);
        seq_t numerator = pow_val + modmp - 1;         /* = p^{E+1} - 1 (mod modmp) */
        seq_t sum = numerator / (seq_t)(p - 1);        /* exact division */
        sum = sum % denom;                            /* reduce modulo mod */

        result = (result * sum) % denom;
    }

    return result;
}

/* ------------------------------------------------------------------ */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but return 0 safely */
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    for (uint64_t k = 1; ; ++k) {
        seq_t sigma = sigma_kpk_mod(k, N);
        if ((sigma % (seq_t)N) == 0)
            return (seq_t)k;
    }
}
