/*
 * A258860  Tenth pi-based arithmetic derivative of n.
 * https://oeis.org/A258860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A258851)^10(n) – 10-fold pi‑based arithmetic derivative
 *            The pi‑based derivative dπ(n) = Σ k·π(p)·n/p for each prime
 *            power p^k || n
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME 200000        /* must exceed sqrt(max intermediate value) */

/* Prime sieve data --------------------------------------------------- */
static uint8_t  is_prime[MAX_PRIME + 1];
static uint16_t pi_of    [MAX_PRIME + 1];
static uint32_t primes   [MAX_PRIME + 1];
static size_t   primes_count = 0;
static bool     primes_computed = false;

/* Initialize prime tables once --------------------------------------- */
static void init_primes(void)
{
    if (primes_computed) return;

    /* Eratosthenes sieve */
    for (int i = 0; i <= MAX_PRIME; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = 0;

    /* Prefix sum π(i) and prime list */
    size_t cnt = 0;
    for (int i = 1; i <= MAX_PRIME; ++i) {
        if (is_prime[i]) ++cnt;
        pi_of[i] = (uint16_t)cnt;
    }
    primes_count = 0;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) primes[primes_count++] = (uint32_t)i;

    primes_computed = true;
}

/* Compute the pi‑based arithmetic derivative of n ------------------- */
static seq_t pi_derivative(seq_t n)
{
    if (n <= 1) return 0;

    /* Work in 64‑bit to factor n (n fits in 64 bits for all n up to 10 000) */
    uint64_t temp = (uint64_t)n;
    seq_t    result = 0;

    for (size_t idx = 0; idx < primes_count &&
                         (uint64_t)primes[idx] * (uint64_t)primes[idx] <= temp; ++idx)
    {
        uint32_t p = primes[idx];
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            /* term = exp * π(p) * n/p */
            seq_t term = (seq_t)exp * (seq_t)pi_of[p] * (n / (seq_t)p);
            result += term;
        }
    }

    if (temp > 1) {   /* remaining factor is prime > sqrt(original n) */
        uint32_t p = (uint32_t)temp;
        seq_t term = (seq_t)1 * (seq_t)pi_of[p] * (n / (seq_t)p);
        result += term;
    }

    return result;
}

/* Sequence element a(n) ------------------------------------------------*/
seq_t a(idx_t n)
{
    init_primes();

    seq_t val = (seq_t)n;
    for (int i = 0; i < 10; ++i)
        val = pi_derivative(val);

    return val;
}
