/*
 * A351681  Stirling transform of {1, primes}.
 * https://oeis.org/A351681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Stirling2(n,k) * s(k), where s(1)=1,
 *            s(k)=prime(k-1) for k>=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    static seq_t *S_prev = NULL;
    static seq_t *S_curr = NULL;
    static idx_t S_cap = 0;

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = 0;
        cache[1] = (seq_t)1;
        cache_size = 1;

        S_cap = 2;
        S_prev = (seq_t *)malloc(sizeof(seq_t) * S_cap);
        S_curr = (seq_t *)malloc(sizeof(seq_t) * S_cap);
        S_prev[0] = 0;
        S_prev[1] = (seq_t)1;
    }

    if (n > cache_size) {
        if (prime_cnt < (size_t)(n - 1)) {
            size_t needed = (size_t)(n - 1);
            size_t alloc = needed + 10;
            uint64_t *tmp = (uint64_t *)malloc(sizeof(uint64_t) * alloc);
            size_t cnt = 0;
            uint64_t num = 2;
            while (cnt < needed) {
                bool is_prime = true;
                for (size_t i = 0; i < cnt; ++i) {
                    uint64_t p = tmp[i];
                    if (p * p > num) break;
                    if (num % p == 0) { is_prime = false; break; }
                }
                if (is_prime) {
                    if (cnt == alloc) {
                        alloc <<= 1;
                        tmp = (uint64_t *)realloc(tmp, sizeof(uint64_t) * alloc);
                    }
                    tmp[cnt++] = num;
                }
                ++num;
            }
            free(primes);
            primes = tmp;
            prime_cnt = cnt;
        }

        if (S_cap < n + 1) {
            S_cap = n + 1;
            S_prev = (seq_t *)realloc(S_prev, sizeof(seq_t) * S_cap);
            S_curr = (seq_t *)realloc(S_curr, sizeof(seq_t) * S_cap);
        }

        if (cache_size + 1 <= n) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        }

        for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
            S_curr[0] = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t term1 = 0;
                if (k <= cur - 1) term1 = S_prev[k] * (seq_t)k;
                seq_t term2 = S_prev[k - 1];
                S_curr[k] = term1 + term2;
            }

            seq_t sum = 0;
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t sk = (k == 1) ? (seq_t)1 : (seq_t)primes[k - 2];
                sum += S_curr[k] * sk;
            }
            cache[cur] = sum;

            seq_t *tmpS = S_prev;
            S_prev = S_curr;
            S_curr = tmpS;
        }

        cache_size = n;
    }

    return cache[n];
}
