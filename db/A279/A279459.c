/*
 * A279459  Numbers n such that sum of the proper divisors of n is the square
 *          of the sum of the digits of n.
 * https://oeis.org/A279459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Numbers n such that the sum of the proper divisors of n
   equals the square of the sum of the digits of n.  The function
   a(idx_t n) returns the nth such number (a(1)=24, a(2)=153, …).  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for the offset; any value is acceptable. */
        return (seq_t)0;
    }

#ifdef __cplusplus
    /* C++ build uses vectors which handle construction automatically. */
    static std::vector<seq_t> cache;
    static std::vector<uint64_t> primes;
    static uint64_t max_checked = 0;

    if (cache.size() >= n) {
        return cache[n - 1];
    }

    for (uint64_t cand = max_checked + 1; ; ++cand) {
        /* Factor cand using previously found primes. */
        uint64_t temp = cand;
        seq_t sigma = (seq_t)1;

        for (size_t i = 0; i < primes.size(); ++i) {
            uint64_t p = primes[i];
            if (p > temp / p) {   /* p*p > temp -> stop */
                break;
            }
            if (temp % p != 0) {
                continue;
            }
            /* Compute sum_{k=0}^{e} p^k for this prime factor. */
            seq_t term     = (seq_t)1;
            seq_t sum_pows = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                term *= (seq_t)p;
                sum_pows += term;
            }
            sigma *= sum_pows;
        }
        if (temp > 1) {   /* temp is a prime factor > sqrt(cand) */
            sigma *= ((seq_t)1 + (seq_t)temp);
        }

        seq_t proper = sigma - (seq_t)cand;

        /* Sum of digits of cand. */
        uint64_t digit_sum = 0;
        uint64_t t = cand;
        while (t > 0) {
            digit_sum += t % 10;
            t /= 10;
        }
        seq_t digit_square = (seq_t)digit_sum * (seq_t)digit_sum;

        if (proper == digit_square) {
            cache.push_back((seq_t)cand);
            if (cache.size() >= n) {
                max_checked = cand;
                return cache[n - 1];
            }
        }

        /* Record new prime if cand itself is prime. */
        if (temp > 1 && (cand == 2 || (primes.size() == 0 || cand % primes.back() != 0))) {
            /* cand is prime if no divisor found (temp remains >1 after loop). */
            if (cand % 2 != 0 || cand == 2) {
                if (cand > 1 && (primes.empty() || cand % primes.back() != 0)) {
                    /* Ensure cand is not divisible by any earlier prime. */
                    bool is_prime = true;
                    for (size_t i = 0; i < primes.size(); ++i) {
                        if (primes[i] > cand / primes[i]) break;
                        if (cand % primes[i] == 0) {
                            is_prime = false;
                            break;
                        }
                    }
                    if (is_prime) {
                        primes.push_back(cand);
                    }
                }
            }
        }

        max_checked = cand;
    }
#else
    /* C build uses raw memory allocation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static uint64_t max_checked = 0;

    if (cache_len >= n) {
        return cache[n - 1];
    }

    for (uint64_t cand = max_checked + 1; ; ++cand) {
        uint64_t temp = cand;
        seq_t sigma = (seq_t)1;

        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > temp / p) {
                break;
            }
            if (temp % p != 0) {
                continue;
            }
            seq_t term     = (seq_t)1;
            seq_t sum_pows = (seq_t)1;
            while (temp % p == 0) {
                temp /= p;
                term *= (seq_t)p;
                sum_pows += term;
            }
            sigma *= sum_pows;
        }
        if (temp > 1) {
            sigma *= ((seq_t)1 + (seq_t)temp);
        }

        seq_t proper = sigma - (seq_t)cand;

        /* Sum of digits of cand. */
        uint64_t digit_sum = 0;
        uint64_t t = cand;
        while (t > 0) {
            digit_sum += t % 10;
            t /= 10;
        }
        seq_t digit_square = (seq_t)digit_sum * (seq_t)digit_sum;

        if (proper == digit_square) {
            /* Grow cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
            if (cache_len >= n) {
                max_checked = cand;
                return cache[n - 1];
            }
        }

        /* Record new prime if cand is prime. */
        if (temp > 1 && (cand == 2 || primes_len == 0 ||
                         cand % primes[primes_len - 1] != 0)) {
            /* Verify cand not divisible by any existing prime. */
            bool is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                if (primes[i] > cand / primes[i]) break;
                if (cand % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                if (primes_len == primes_cap) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                    primes = tmp;
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;
            }
        }

        max_checked = cand;
    }
#endif
}
