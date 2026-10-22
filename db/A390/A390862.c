/*
 * A390862  Number of positive integers k such that at least 1/n of the
 *          integers from 1 to k are divisors of k.
 * https://oeis.org/A390862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute a(n) = |{k ∈ ℕ : d(k) ≥ k/n}|
 *   where d(k) = number of divisors of k.
 * The routine counts every k up to 4·n² – the theoretical bound –
 * and updates a running cache so that later calls are answered in
 * O(1).
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* integer square‑root (floor) for idx_t */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1U) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1U;
    }
    return low;
}

/* cache of the values a(1)…a(Nmax) (0‑indexed, a_cache[0]=0) */
static seq_t *seq_cache = NULL;   /* allocate with new[] in C++, malloc in C */
static idx_t seq_cache_max = 0;    /* maximal n cached */

static void build_cache(idx_t Nmax)
{
    /* free any previous cache */
    if (seq_cache) {
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
        seq_cache = NULL;
    }
#ifdef __cplusplus
    seq_cache = new seq_t[Nmax + 1];
#else
    seq_cache = (seq_t*)malloc((Nmax + 1) * sizeof(seq_t));
#endif
    if (!seq_cache) return;

    /* delta[i] counts how many k have ceil(k/d(k)) == i */
    size_t *delta = (size_t*)calloc((size_t)(Nmax + 1), sizeof(size_t));
    if (!delta) {
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
        seq_cache = NULL;
        return;
    }

    idx_t limit   = 4ULL * Nmax * Nmax;              /* maximal k to check   */
    idx_t sqrt_lim = isqrt_u64(limit);               /* upper bound for primes */

    /* simple sieve of Eratosthenes up to sqrt_lim */
    bool  *is_prime = (bool*)malloc((sqrt_lim + 1) * sizeof(bool));
    if (!is_prime) {
        free(delta);
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
        seq_cache = NULL;
        return;
    }
    for (idx_t i = 2; i <= sqrt_lim; ++i) is_prime[i] = true;

    uint32_t *primes = (uint32_t*)malloc((sqrt_lim + 1) * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        free(delta);
#ifdef __cplusplus
        delete[] seq_cache;
#else
        free(seq_cache);
#endif
        seq_cache = NULL;
        return;
    }
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= sqrt_lim) {
                for (idx_t j = i * i; j <= sqrt_lim; j += i)
                    is_prime[j] = false;
            }
        }
    }
    free(is_prime);

    /* segmented divisor counting up to limit */
    const idx_t BLOCK = 1 << 20;   /* 1,048,576 numbers per block */
    for (idx_t L = 1; L <= limit; L += BLOCK) {
        idx_t R = L + BLOCK - 1;
        if (R > limit) R = limit;
        size_t sz = (size_t)(R - L + 1);

        uint32_t *num = (uint32_t*)malloc(sz * sizeof(uint32_t));
        uint16_t *d   = (uint16_t*)malloc(sz * sizeof(uint16_t));
        if (!num || !d) {
            free(num);
            free(d);
            free(primes);
            free(delta);
#ifdef __cplusplus
            delete[] seq_cache;
#else
            free(seq_cache);
#endif
            seq_cache = NULL;
            return;
        }

        for (size_t i = 0; i < sz; ++i) {
            num[i] = (uint32_t)(L + i);
            d[i]   = 1;
        }

        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint32_t p = primes[pi];
            idx_t start = (L + p - 1U) / p * p;
            for (idx_t j = start; j <= R; j += p) {
                size_t idx = (size_t)(j - L);
                uint32_t cnt = 0;
                while (num[idx] % p == 0U) {
                    num[idx] /= p;
                    ++cnt;
                }
                if (cnt) d[idx] *= (cnt + 1U);
            }
        }

        for (size_t i = 0; i < sz; ++i)
            if (num[i] > 1U)
                d[i] *= 2U;    /* remaining prime factor */

        for (size_t i = 0; i < sz; ++i) {
            uint32_t k  = (uint32_t)(L + i);
            uint32_t dk = d[i];
            idx_t n0 = (idx_t)((k + dk - 1U) / dk);
            if (n0 <= Nmax) delta[n0] += 1;
        }

        free(num);
        free(d);
    }

    /* prefix sum to obtain the sequence values */
    seq_cache[0] = 0;
    for (idx_t i = 1; i <= Nmax; ++i)
        seq_cache[i] = seq_cache[i-1] + (seq_t)delta[i];

    free(delta);
    free(primes);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */
    if (n > seq_cache_max) {
        build_cache(n);
        seq_cache_max = n;
    }
    return seq_cache[n];
}
