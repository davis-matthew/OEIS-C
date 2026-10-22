/*
 * A358674  Numbers k for which there is a factorization of k into such a pair
 *          of natural numbers x and y, that the sum (x * y') + (x' * y) will
 *          generate at least one carry when the addition is done in the
 *          primorial base. Here n' stands for A003415(n), the arithmetic
 *          derivative of n.
 * https://oeis.org/A358674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A358674 by brute‑force search of factor pairs and
 * primorial‑base carry detection. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef MAX_PRIMES
#define MAX_PRIMES 2048   /* enough primes for the required ranges */
#endif

/* Static state for the prime table used in the carry test and
 * in the arithmetic derivative. */
static bool primes_initialized = false;
static uint32_t primes[MAX_PRIMES];
static size_t primes_count = 0;

/* Sieve of Eratosthenes to initialise a table of the first few primes. */
static void init_primes(void)
{
    if (primes_initialized) return;
    const uint32_t LIMIT = 30000;          /* > 2000th prime */
    bool *is_composite = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (uint32_t i = 0; i <= LIMIT; ++i) is_composite[i] = false;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_count < MAX_PRIMES) primes[primes_count++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);
    primes_initialized = true;
}

/* Arithmetic derivative A003415 of a number n (seq_t). */
static seq_t derivative(seq_t n)
{
    if (n <= (seq_t)1) return (seq_t)0;
    seq_t original = n;
    seq_t res = (seq_t)0;
    init_primes();
    for (size_t i = 0; i < primes_count; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > n) break;
        int exp = 0;
        while (n % p == (seq_t)0) {
            n = n / p;
            ++exp;
        }
        if (exp > 0) {
            res = res + (original / p) * exp;
        }
    }
    if (n > (seq_t)1) {
        /* Remaining factor is prime */
        res = res + (original / n);
    }
    return res;
}

/* Detects whether adding a and b in primorial base produces
 * at least one carry. */
static bool has_carry(seq_t a, seq_t b)
{
    init_primes();
    seq_t carry = (seq_t)0;
    size_t idx = 0;
    while (a != (seq_t)0 || b != (seq_t)0 || carry != (seq_t)0) {
        if (idx >= primes_count) {
            /* Should not happen for the numbers encountered. */
            return false;
        }
        seq_t p = (seq_t)primes[idx++];
        seq_t da = a % p;
        seq_t db = b % p;
        seq_t sum = da + db + carry;
        if (sum >= p) return true;
        carry = sum / p;
        a = a / p;
        b = b / p;
    }
    return false;
}

/* Tests whether the integer k has a factorisation satisfying the
 * definition of A358674. */
static bool member(seq_t k)
{
    seq_t x = (seq_t)1;
    while (x * x <= k) {
        if (k % x == (seq_t)0) {
            seq_t y = k / x;
            seq_t x_der = derivative(x);
            seq_t y_der = derivative(y);
            seq_t a = x * y_der;
            seq_t b = x_der * y;
            if (has_carry(a, b)) return true;
        }
        x = x + (seq_t)1;
    }
    return false;
}

/* Dynamic array bookkeeping for memoisation of sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

static idx_t next_candidate = 8;   /* first term known from OEIS */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_candidate;
        if (member(val)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
