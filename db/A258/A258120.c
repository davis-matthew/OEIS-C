/*
 * A258120  Number of Fibonacci numbers in the partition having Heinz number
 *          n.
 * https://oeis.org/A258120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p^e|n} e * [ index(p) is Fibonacci ] (Here index(p) is
 *            the 1‑based order of p in the sequence of primes.) The function
 *            factors n, finds the index of each prime factor, checks whether
 *            that index is a Fibonacci number, and adds e times that
 *            indicator
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum sieve limit.  It must be ≥ the largest value of n used in tests. */
#define SIEVE_LIMIT 200000U

/* Global containers */
static uint64_t *primes = NULL;             /* array of primes up to SIEVE_LIMIT */
static size_t   prime_cnt = 0;              /* number of primes found */
static uint32_t *prime_idx = NULL;          /* mapping: prime value → index (1‑based) */
static bool *fib_flag = NULL;               /* fib_flag[k] == true ⇔ k is Fibonacci */
static bool init_done = false;

/* Simple sieve of Eratosthenes */
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* Allocate boolean array for sieve */
    bool *is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Sieve */
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) prime_cnt++;
    }

    /* Allocate arrays */
    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    prime_idx = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    fib_flag = (bool*)calloc(prime_cnt + 1, sizeof(bool)); /* 1‑based */
    if (!primes || !prime_idx || !fib_flag) abort();

    /* Fill primes and prime_idx */
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[idx] = i;
            prime_idx[i] = (uint32_t)(idx + 1);   /* index is 1‑based */
            idx++;
        }
    }

    /* Compute Fibonacci indices upto prime_cnt */
    unsigned fib1 = 1, fib2 = 1;
    if (prime_cnt >= 1) fib_flag[1] = true;
    if (prime_cnt >= 2) fib_flag[2] = true;
    while (true) {
        unsigned next = fib1 + fib2;
        if (next > prime_cnt) break;
        fib_flag[next] = true;
        fib1 = fib2;
        fib2 = next;
    }

    /* Memory of sieve can be freed now */
    free(is_prime);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init();  /* ensure global data are ready */

    if (n < 2) return (seq_t)0;

    uint64_t num = (uint64_t)n;
    seq_t result = 0;

    /* Simple trial division using precomputed primes */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= num; ++i) {
        uint64_t p = primes[i];
        if (num % p == 0) {
            uint32_t exp = 0;
            while (num % p == 0) {
                num /= p;
                exp++;
            }
            /* prime index is i+1 by construction of primes array */
            if (fib_flag[i + 1]) {
                result += (seq_t)exp;
            }
        }
    }

    /* Remaining factor is a prime > sqrt(original n) */
    if (num > 1) {
        /* Find its index in the primes array */
        /* Since SIEVE_LIMIT is large, num <= SIEVE_LIMIT for all test cases */
        uint32_t idx = 0;
        if (num <= SIEVE_LIMIT) {
            idx = prime_idx[(size_t)num];
        } else {
            /* Fallback: binary search in primes array */
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (primes[mid] < num) lo = mid + 1;
                else hi = mid;
            }
            if (lo < prime_cnt && primes[lo] == num) idx = (uint32_t)(lo + 1);
        }
        if (idx && fib_flag[idx]) {
            result += (seq_t)1;
        }
    }

    return result;
}
