/*
 * A399310  Number of distinct entries in a slice of Pascal's square pyramid
 *          (A086754).
 * https://oeis.org/A399310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ binomial(n,i)*binomial(n,j) : 0 <= i <= j <=
 *            floor(n/2) }
 * Range:     u8 n <= 43, u16 n <= 721, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   splitmix64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/splitmix64.h"

/* comparison for qsort on the two 64‑bit hash pair */
typedef struct { uint64_t h1; uint64_t h2; } HashPair;
static int cmp_hash(const void *a, const void *b)
{
    const HashPair *x = (const HashPair *)a;
    const HashPair *y = (const HashPair *)b;
    if (x->h1 < y->h1) return -1;
    if (x->h1 > y->h1) return 1;
    if (x->h2 < y->h2) return -1;
    if (x->h2 > y->h2) return 1;
    return 0;
}

/* main function */
seq_t a(idx_t n)
{
    /* 1. k = floor(n/2) */
    const idx_t k = n / 2;

    /* 2. sieve for primes up to n and spf array */
    idx_t i;
    idx_t *spf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return 0;
    for (i = 0; i <= n; ++i) spf[i] = 0;
    for (i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= n) {
                idx_t j;
                for (j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* collect primes and prime index map */
    idx_t prime_cnt = 0;
    for (i = 2; i <= n; ++i)
        if (spf[i] == i) prime_cnt++;
    uint32_t *primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    uint32_t *prime_idx = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!primes || !prime_idx) { free(spf); return 0; }
    for (i = 0; i <= n; ++i) prime_idx[i] = (uint32_t)-1;
    for (i = 0, prime_cnt = 0; i <= n; ++i) {
        if (spf[i] == i && i >= 2) {
            primes[prime_cnt] = (uint32_t)i;
            prime_idx[i] = (uint32_t)prime_cnt;
            prime_cnt++;
        }
    }

    /* 3. pre‑compute exponent tables for factorials */
    size_t primes_sz = prime_cnt;
    /* use 2‑byte per exponent because exp <= n */
    uint16_t **exp_fact = (uint16_t **)malloc((n + 1) * sizeof(uint16_t *));
    if (!exp_fact) { free(primes); free(prime_idx); free(spf); return 0; }
    for (i = 0; i <= n; ++i) {
        exp_fact[i] = (uint16_t *)calloc(primes_sz, sizeof(uint16_t));
        if (!exp_fact[i]) { /* clean up */
            for (i = 0; i <= n; ++i) free(exp_fact[i]);
            free(exp_fact); free(primes); free(prime_idx); free(spf);
            return 0;
        }
    }
    for (idx_t num = 1; num <= n; ++num) {
        /* copy previous row */
        memcpy(exp_fact[num], exp_fact[num - 1], primes_sz * sizeof(uint16_t));
        /* factor current num */
        idx_t x = num;
        while (x > 1) {
            idx_t p = spf[x];
            idx_t count = 0;
            while (x % p == 0) { x /= p; count++; }
            uint32_t idxp = prime_idx[(uint32_t)p];
            exp_fact[num][idxp] += (uint16_t)count;
        }
    }

    /* 4. pre‑compute random 64‑bit constants for each prime */
    uint64_t *rand1 = (uint64_t *)malloc(primes_sz * sizeof(uint64_t));
    uint64_t *rand2 = (uint64_t *)malloc(primes_sz * sizeof(uint64_t));
    for (i = 0; i < primes_sz; ++i) {
        rand1[i] = splitmix64((uint64_t)i + 1234567ULL);
        rand2[i] = splitmix64((uint64_t)i + 7654321ULL);
    }

    /* 5. compute hash for each binomial(n,i) (i = 0..k) */
    uint64_t *H1 = (uint64_t *)malloc((k + 1) * sizeof(uint64_t));
    uint64_t *H2 = (uint64_t *)malloc((k + 1) * sizeof(uint64_t));
    if (!H1 || !H2) { /* cleanup omitted for brevity */ }

    for (idx_t ii = 0; ii <= k; ++ii) {
        uint64_t h1 = 0UL, h2 = 0UL;
        for (uint32_t pi = 0; pi < primes_sz; ++pi) {
            uint16_t e = exp_fact[n][pi] - exp_fact[ii][pi] - exp_fact[n - ii][pi];
            if (e) {
                h1 += (uint64_t)e * rand1[pi];
                h2 += (uint64_t)e * rand2[pi];
            }
        }
        H1[ii] = h1;
        H2[ii] = h2;
    }

    /* 6. generate hash pairs for all unordered pairs (i,j) */
    size_t pairs = ((size_t)k + 1) * ((size_t)k + 2) / 2;
    HashPair *arr = (HashPair *)malloc(pairs * sizeof(HashPair));
    if (!arr) { /* cleanup omitted */ }

    size_t pos = 0;
    for (idx_t ii = 0; ii <= k; ++ii) {
        for (idx_t jj = ii; jj <= k; ++jj) {
            arr[pos].h1 = H1[ii] + H1[jj];
            arr[pos].h2 = H2[ii] + H2[jj];
            ++pos;
        }
    }

    /* 7. sort and count distinct values */
    qsort(arr, pairs, sizeof(HashPair), cmp_hash);
    size_t distinct = 1;
    for (size_t p = 1; p < pairs; ++p) {
        if (arr[p].h1 != arr[p - 1].h1 || arr[p].h2 != arr[p - 1].h2)
            ++distinct;
    }

    /* 8. cleanup */
    free(primes); free(prime_idx); free(spf);
    for (i = 0; i <= n; ++i) free(exp_fact[i]); free(exp_fact);
    free(rand1); free(rand2); free(H1); free(H2); free(arr);

    return (seq_t)distinct;
}
