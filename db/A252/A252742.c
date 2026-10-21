/*
 * A252742  Characteristic function of A246282: if A003961(n) > 2*n, then a(n)
 *          = 1, otherwise 0 (when A003961(n) < 2*n) [where A003961(n) shifts
 *          the prime factorization of n one step towards larger primes].
 * https://oeis.org/A252742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = [A003961(n) > 2*n]
 * Range:     u8 n <= 55, u16 n <= 6983, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Prime list up to 20000 – sufficient for n ≤ 10000 */
static uint64_t primes[3000];
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void) {
    if (primes_initialized) return;
    const size_t LIMIT = 20000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) primes[prime_count++] = i;
    }
    free(is_prime);
    primes_initialized = true;
}

/* Next prime after p (p must be prime) */
static uint64_t next_prime_of(uint64_t p) {
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (lo < prime_count && primes[lo] == p) {
        if (lo + 1 < prime_count) return primes[lo + 1];
    }
    return 0; /* Should never happen with valid input */
}

/* Compute A003961(n) = product of next_prime(p)^e for each prime p^e in n */
static seq_t A003961_value(uint64_t n) {
    seq_t prod = 1;
    uint64_t m = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) break;
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            uint64_t q = next_prime_of(p);
            seq_t powq = 1;
            for (int j = 0; j < exp; ++j) powq *= (seq_t)q;
            prod *= powq;
        }
    }
    if (m > 1) {             /* remaining prime factor */
        uint64_t q = next_prime_of(m);
        prod *= (seq_t)q;
    }
    return prod;
}

seq_t a(idx_t n) {
    init_primes();
    if (n < 1) return 0;
    seq_t val = A003961_value((uint64_t)n);
    return (val > (seq_t)n * 2) ? 1 : 0;
}
