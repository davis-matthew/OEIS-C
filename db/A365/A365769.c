/*
 * A365769  Lexicographically earliest permutation of the prime numbers such
 *          that a(n) - [the next digit] is a nonprime number and a(n) - [the
 *          next digit] >= 0.
 * https://oeis.org/A365769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th term of the lexicographically earliest *
 *            permutation of the prime numbers such that for each consecutive
 *            pair * p_k, p_{k+1} the difference p_k - first_digit(p_{k+1}) is
 *            a * non‑prime number and ≥ 0. The sequence is generated greedily
 *            by * choosing the smallest unused prime that...
 * Range:     u8 n <= 46, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order calls */
    static bool initialized = false;
    static seq_t *primes = NULL;
    static unsigned char *first_digit = NULL;
    static bool *used_global = NULL;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t primes_cnt = 0;

    if (n == 0)
        return 0;                       /* offset 1 – n must be ≥1 */

    /* Lazy initialization: build primes up to a static bound */
    if (!initialized) {
        /* 200 000 is large enough for the first 20 000 primes,
         * more than sufficient for n ≤ 10 000. */
        const size_t UPPER = 200000;
        bool *is_prime = (bool*)malloc((UPPER + 1) * sizeof(bool));
        if (!is_prime)
            return 0;
        for (size_t i = 0; i <= UPPER; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t i = 2; i * i <= UPPER; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= UPPER; j += i)
                    is_prime[j] = false;

        /* Count primes */
        for (size_t i = 2; i <= UPPER; ++i)
            if (is_prime[i]) ++primes_cnt;

        /* Allocate arrays */
        primes = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
        first_digit = (unsigned char*)malloc(primes_cnt * sizeof(unsigned char));
        used_global = (bool*)calloc(primes_cnt, sizeof(bool));
        cache = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
        if (!primes || !first_digit || !used_global || !cache) {
            free(primes); free(first_digit); free(used_global); free(cache); free(is_prime);
            return 0;
        }

        /* Fill primes and first_digit arrays */
        size_t idx = 0;
        for (size_t i = 2; i <= UPPER; ++i) {
            if (!is_prime[i])
                continue;
            primes[idx] = (seq_t)i;          /* cast works for both C++ and C  */
            uint64_t x = i;
            while (x >= 10) x /= 10;
            first_digit[idx] = (unsigned char)x;
            ++idx;
        }

        free(is_prime);
        cache_size = 0;
        initialized = true;
    }

    /* Extend the cache if necessary */
    while (cache_size < (size_t)n) {
        if (cache_size == 0) {                /* first element */
            size_t i = 0;
            while (i < primes_cnt && used_global[i])
                ++i;
            if (i == primes_cnt)
                return 0;                    /* should not happen */
            cache[0] = primes[i];
            used_global[i] = true;
            ++cache_size;
        } else {
            seq_t prev = cache[cache_size - 1];
            size_t chosen = primes_cnt;       /* sentinel: not found */
            for (size_t i = 0; i < primes_cnt; ++i) {
                if (used_global[i])
                    continue;
                seq_t digit = (seq_t)first_digit[i];
                if (prev < digit)
                    continue;
                seq_t diff = prev - digit;
                if (!is_prime_big(diff)) {
                    chosen = i;
                    break;                    /* lexicographically earliest */
                }
            }
            if (chosen == primes_cnt)
                return 0;                    /* sequence definition violated */
            cache[cache_size] = primes[chosen];
            used_global[chosen] = true;
            ++cache_size;
        }
    }

    return cache[n - 1];
}
