/*
 * A353331  a(n) = 1 if A001222(n) [bigomega(n)] and A056239(n) are both even,
 *          otherwise a(n) = 0.
 * https://oeis.org/A353331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A001222(n) (bigomega) and A056239(n) (sum of prime *
 *            indices of the prime factorization) are both even; otherwise 0
 * Range:     u8 n <= 9695, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: offset is 1 (a(1) is the first term). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Utility functions ---------- */

/* Integer square root: floor(sqrt(n)).  Uses only integer arithmetic. */
static idx_t isqrt_u64(idx_t n) {
    idx_t res = 0;
    idx_t bit = 1ULL << 62;           /* largest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* ---- Prime table generation ---- */
static uint64_t *prime_table = NULL;
static size_t prime_count   = 0;
static idx_t prime_limit    = 0;

/* Generate all primes ≤ limit and store them in prime_table. */
static void ensure_primes_up_to(idx_t limit) {
    if (limit <= prime_limit) return;   /* already enough */

    /* Sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (!is_prime) return;             /* allocation failure – silently ignore */

    memset(is_prime, 1, sz);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p <= limit / p; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p) {
                is_prime[q] = 0;
            }
        }
    }

    /* Count primes and copy them */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++cnt;
    }
    uint64_t *new_table = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_table) {
        free(is_prime);
        return;                       /* allocation failure – silently ignore */
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) new_table[pos++] = (uint64_t)i;
    }

    free(is_prime);
    free(prime_table);
    prime_table = new_table;
    prime_count = cnt;
    prime_limit = limit;
}

/* Return 1‑based index of prime p in the prime list.  Assumes p is prime. */
static idx_t prime_index(uint64_t p) {
    ensure_primes_up_to(p);
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_table[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (lo < prime_count && prime_table[lo] == p)
        return (idx_t)(lo + 1);
    /* Should never happen for a prime factor of n. */
    return (idx_t)0;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined offset – return 0 */

    uint64_t temp = (uint64_t)n;
    int bigomega_parity = 0;        /* 0 for even, 1 for odd */
    int index_sum_parity = 0;       /* parity of sum of prime indices */

    idx_t sqrt_lim = isqrt_u64(temp);
    ensure_primes_up_to(sqrt_lim);

    for (size_t i = 0; i < prime_count && prime_table[i] <= sqrt_lim; ++i) {
        uint64_t p = prime_table[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp & 1) bigomega_parity ^= 1;
            /* parity of (prime index * exp) is (index mod 2) & (exp mod 2) */
            if (( ((idx_t)(i + 1) & 1) & (exp & 1) )) index_sum_parity ^= 1;

            sqrt_lim = isqrt_u64(temp);
        }
    }

    if (temp > 1) {                 /* remaining prime factor */
        uint64_t p = temp;
        idx_t idx = prime_index(p); /* 1‑based prime index */
        bigomega_parity ^= 1;           /* exponent is 1 (odd) */
        if (idx & 1) index_sum_parity ^= 1;
    }

    return (bigomega_parity == 0 && index_sum_parity == 0) ? (seq_t)1 : (seq_t)0;
}
