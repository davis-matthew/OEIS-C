/*
 * A302697  Odd numbers whose prime indices are relatively prime. Heinz
 *          numbers of integer partitions with no 1's and with relatively
 *          prime parts.
 * https://oeis.org/A302697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer whose prime indices are pairwise
 *            coprime (Heinz numbers of integer partitions with no 1's and
 *            with relatively prime parts)
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset */

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static prime data for factorisation */
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_limit = 0;

    /* static candidate odd number */
    static uint64_t candidate = 3;

    /* initialize cache */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
    }

    /* initialize prime list */
    if (!prime_arr) {
        prime_limit = 1000;
        build_primes_upto(prime_limit, &prime_arr, &prime_cnt);
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t m = candidate;
        seq_t g = 0;   /* gcd of prime indices seen so far */

        uint64_t temp = m;

        /* trial division using primes up to sqrt(temp) */
        for (size_t i = 0; i < prime_cnt && prime_arr[i] <= temp / prime_arr[i]; ++i) {
            uint64_t p = prime_arr[i];
            if (temp % p == 0) {
                seq_t idx = (seq_t)(i + 1);   /* prime index (1‑based) */

                if (g == 0)
                    g = idx;
                else
                    g = gcd(g, idx);

                while (temp % p == 0) temp /= p;   /* strip factor */
            }
        }

        /* remaining factor (if any) is prime */
        if (temp > 1) {
            if (temp > prime_limit) {
                uint64_t new_limit = prime_limit * 2;
                while (new_limit < temp) new_limit *= 2;
                free(prime_arr);
                build_primes_upto(new_limit, &prime_arr, &prime_cnt);
                prime_limit = new_limit;
            }

            /* find its index in the prime array */
            size_t pos = 0;
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (prime_arr[mid] == temp) {
                    pos = mid;
                    break;
                }
                if (prime_arr[mid] < temp)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            seq_t idx = (seq_t)(pos + 1);
            if (g == 0)
                g = idx;
            else
                g = gcd(g, idx);
        }

        if (g == 1) {
            /* accept this odd integer */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)m;
        }

        candidate += 2;

        /* ensure prime table covers next candidate (for future factors) */
        if (candidate > prime_limit) {
            uint64_t new_limit = prime_limit * 2;
            while (new_limit < candidate) new_limit *= 2;
            free(prime_arr);
            build_primes_upto(new_limit, &prime_arr, &prime_cnt);
            prime_limit = new_limit;
        }
    }

    return cache[n-1];
}
