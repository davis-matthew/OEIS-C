/*
 * A293448  Self-inverse permutation of natural numbers: replace (with
 *          multiplicity) each prime factor A000040(k) with
 *          A000040(min+(max-k)) in the prime factorization of n, where min =
 *          A055396(n) and max = A061395(n).
 * https://oeis.org/A293448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ prime(min+(max-index(p)))^exp, where n = ∏
 *            prime(index(p))^exp, min/max are the indices of the
 *            smallest/largest prime divisor of n
 * Range:     u8 n <= 51, u16 n <= 271, u32 n <= 2367, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/factor_u64.h"

/* Prime sieve helpers ------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t prime_used   = 0;
static size_t prime_cap    = 0;

/* Ensure that prime_list contains all primes up to the value 'limit'. */
static void ensure_primes_up_to_value(uint64_t limit) {
    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_used = 0;
    }
    uint64_t cand = (prime_used == 0) ? 2ULL : prime_list[prime_used - 1] + 1;
    while (cand <= limit) {
        bool isprime = true;
        uint64_t r = (uint64_t)sqrt((long double)cand);
        for (size_t i = 0; i < prime_used && prime_list[i] <= r; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_used == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_used++] = cand;
        }
        ++cand;
    }
}

/* Ensure that prime_list contains at least 'count' primes (indices 1..count). */
static void ensure_primes_up_to_index(uint64_t count) {
    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_used = 0;
    }
    uint64_t cand = (prime_used == 0) ? 2ULL : prime_list[prime_used - 1] + 1;
    while (prime_used < count) {
        bool isprime = true;
        uint64_t r = (uint64_t)sqrt((long double)cand);
        for (size_t i = 0; i < prime_used && prime_list[i] <= r; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            if (prime_used == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_used++] = cand;
        }
        ++cand;
    }
}

/* Return the index (1‑based) of prime 'p'. */
static uint64_t prime_index(uint64_t p) {
    ensure_primes_up_to_value(p);
    /* Binary search for p. */
    size_t lo = 0, hi = prime_used;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return mid + 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* should never happen */
}

/* Return the prime with 1‑based index k. */
static uint64_t prime_by_index(uint64_t k) {
    ensure_primes_up_to_index(k);
    return prime_list[k - 1];
}

/* Compute base^exp (exp is small). */
static seq_t pow_small(seq_t base, uint64_t exp) {
    seq_t res = 1;
    for (uint64_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 1)
        return 1;                     /* a(1) = 1 */

    /* Factor n.  Factor arrays sized for at most 64 distinct primes. */
    uint64_t fac_pr[64];
    uint64_t fac_exp[64];
    size_t    fac_cnt = 0;

    factor_u64((uint64_t)n, fac_pr, fac_exp, &fac_cnt);

    uint64_t min_idx = ~0ULL;
    uint64_t max_idx =  0ULL;
    /* Compute indices of prime factors. */
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t idx = prime_index(fac_pr[i]);
        if (idx < min_idx) min_idx = idx;
        if (idx > max_idx) max_idx = idx;
    }

    seq_t result = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t k   = prime_index(fac_pr[i]);   /* original index */
        uint64_t mapped = min_idx + (max_idx - k);  /* reverse mapping */
        uint64_t newp  = prime_by_index(mapped);
        seq_t power_new = pow_small((seq_t)newp, fac_exp[i]);
        result *= power_new;
    }
    return result;
}
