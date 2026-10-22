/*
 * A400468  Numbers k such that k+1 divides phi(k)^2, where phi is Euler's
 *          totient function (A000010).
 * https://oeis.org/A400468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400468: numbers k such that k+1 divides phi(k)^2. */
/* Generates terms sequentially and caches the first 10000 values. */

#include <stdint.h>
#include <stdbool.h>

static seq_t seq[10000];      /* cache of terms, 1-indexed via offset */
static idx_t terms_count = 0; /* how many terms stored */
static uint64_t current_k = 1;/* last tested k */

/* small prime sieve up to 200000 (enough for sqrt of k up to ~4*10^10) */
#define PRIME_MAX 200000
static uint32_t primes[PRIME_MAX];
static idx_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool is_comp[PRIME_MAX + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_MAX; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_MAX) {
                for (uint32_t j = i * i; j <= PRIME_MAX; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* compute Euler's totient of n using the prime list */
static uint64_t phi_calc(uint64_t n)
{
    uint64_t result = n;
    uint64_t temp = n;
    for (idx_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            result -= result / p;
            do { temp /= p; } while (temp % p == 0);
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

/* generate terms until we have at least n terms */
static void ensure_terms(idx_t n)
{
    if (terms_count >= n) return;
    init_primes();
    while (terms_count < n) {
        ++current_k;
        uint64_t phi = phi_calc(current_k);
        __uint128_t sq = (__uint128_t)phi * (__uint128_t)phi;
        if (sq % (current_k + 1) == 0) {
            seq[terms_count++] = (seq_t)current_k;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq[n - 1];
}
