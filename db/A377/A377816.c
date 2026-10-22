/*
 * A377816  Numbers that have a single even exponent in their prime
 *          factorization.
 * https://oeis.org/A377816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute force: numbers with exactly one even exponent in their prime factorization. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility function prototypes (provided elsewhere). */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint64_t max_tested = 0;

static bool *is_prime = NULL;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL) return;
    const size_t LIMIT = 2000000;          /* enough for all inspected values */
    is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Allocate space for primes (maximum about LIMIT/ln(LIMIT)). */
    primes = (uint64_t *)malloc((LIMIT + 1) * sizeof(uint64_t));
    if (!primes) abort();
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) {
            if (primes_cnt == 0)
                primes_cnt = 1, primes[0] = i;
            else {
                ++primes_cnt;
                primes[primes_cnt - 1] = i;
            }
        }
}

static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap < need) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

static bool has_single_even_exp(uint64_t n)
{
    if (n < 4) return false; /* smallest such number is 4 */
    int evencnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if (exp) {
            if (exp % 2 == 0) {
                ++evencnt;
                if (evencnt > 1) return false;
            }
        }
    }
    if (tmp > 1) { /* remaining prime factor with exponent 1 (odd) */
        /* nothing to do */
    }
    return evencnt == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but harmless */
    if (seq == NULL) {    /* lazy initialisation */
        init_primes();
    }
    while (seq_len < (size_t)n) {
        uint64_t candidate = max_tested + 1;
        max_tested = candidate;
        if (has_single_even_exp(candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)candidate;
        }
    }
    return seq[n - 1];
}
