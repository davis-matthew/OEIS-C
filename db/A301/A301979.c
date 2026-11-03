/*
 * A301979  Number of subset-sums minus number of subset-products of the
 *          integer partition with Heinz number n.
 * https://oeis.org/A301979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A299701(n) - A301957(n)
 * Range:     i8 n <= 16384, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 8565 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ----- Prime sieve ----- */
static uint32_t *prime_list   = NULL;      /* array of primes */
static size_t   prime_count   = 0;         /* number of primes stored */

static void init_primes(void)
{
    if (prime_list != NULL) return;

    /* Sieve up to 2 000 000 – more than enough for n ≤ 10 000 */
    const unsigned int LIM = 2000000;
    bool *is_prime = (bool*)malloc(LIM + 1);
    if (!is_prime) return;

    for (unsigned int i = 0; i <= LIM; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (unsigned int p = 2; (unsigned long long)p * p <= LIM; ++p)
        if (is_prime[p])
            for (unsigned int m = p * p; m <= LIM; m += p)
                is_prime[m] = false;

    for (unsigned int i = 2; i <= LIM; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) { free(is_prime); return; }

    size_t k = 0;
    for (unsigned int i = 2; i <= LIM; ++i)
        if (is_prime[i]) prime_list[k++] = i;

    free(is_prime);
}

/* 1‑based index of prime p in prime_list */
static inline size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else                     hi = mid;
    }
    return lo + 1;
}

/* Comparator for qsort on seq_t elements */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t*)a;
    const seq_t *y = (const seq_t*)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    static bool primes_inited = false;
    if (!primes_inited) { init_primes(); primes_inited = true; }

    /* ---- Factorize n and build the partition (indices of primes) ---- */
    uint64_t temp = n;
    size_t parts_cap = 8;
    size_t parts_len = 0;
    uint32_t *parts = (uint32_t*)malloc(parts_cap * sizeof(uint32_t));
    if (!parts) return 0;

    if (temp > 1) {
        for (size_t pi = 0; pi < prime_count && (uint64_t)prime_list[pi] * prime_list[pi] <= temp; ++pi) {
            uint32_t p = prime_list[pi];
            while (temp % p == 0) {
                if (parts_len == parts_cap) {
                    parts_cap <<= 1;
                    uint32_t *tmp = (uint32_t*)realloc(parts, parts_cap * sizeof(uint32_t));
                    if (!tmp) { free(parts); return 0; }
                    parts = tmp;
                }
                parts[parts_len++] = (uint32_t)prime_index(p);
                temp /= p;
            }
        }
        if (temp > 1) {
            if (parts_len == parts_cap) {
                parts_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(parts, parts_cap * sizeof(uint32_t));
                if (!tmp) { free(parts); return 0; }
                parts = tmp;
            }
            parts[parts_len++] = (uint32_t)prime_index((uint32_t)temp);
        }
    }

    /* ---- Count distinct subset‑sums ---- */
    int sum_total = 0;
    for (size_t i = 0; i < parts_len; ++i) sum_total += (int)parts[i];

    unsigned char *reach = (unsigned char*)calloc((size_t)sum_total + 1, 1);
    if (!reach) { free(parts); return 0; }

    reach[0] = 1;
    for (size_t i = 0; i < parts_len; ++i) {
        int v = (int)parts[i];
        for (int s = sum_total; s >= v; --s)
            if (reach[s - v]) reach[s] = 1;
    }
    int sum_count = 0;
    for (int s = 0; s <= sum_total; ++s)
        if (reach[s]) ++sum_count;
    free(reach);

    /* ---- Count distinct subset‑products ----
       The set S of products starts as {1}; each part v makes it S u vS.
       S is kept sorted, and vS is too, so a merge with duplicates dropped
       does it. (This used to list all 2^parts products in a fixed array of
       8192, which n = 2^13 already filled.) */
    size_t cnt = 1, scap = 16;
    seq_t *S = mem_new(seq_t, scap), *T = mem_new(seq_t, 2 * scap);
    if (!S || !T) abort();
    S[0] = (seq_t)1;
    for (size_t i = 0; i < parts_len; ++i) {
        seq_t v = (seq_t)parts[i];
        if (2 * cnt > 2 * scap) abort();
        size_t p = 0, q = 0, m = 0;
        while (p < cnt || q < cnt) {
            seq_t x;
            if (q == cnt || (p < cnt && S[p] < S[q] * v)) x = S[p++];
            else x = S[q++] * v;
            if (m == 0 || T[m - 1] != x) T[m++] = x;
        }
        if (m > scap) {
            while (scap < m) scap *= 2;
            seq_t *s2 = mem_resize(seq_t, S, scap), *t2 = mem_resize(seq_t, T, 2 * scap);
            if (!s2 || !t2) abort();
            S = s2;
            T = t2;
        }
        for (size_t k = 0; k < m; ++k) S[k] = T[k];
        cnt = m;
    }
    int prod_count = (int)cnt;
    mem_free(S);
    mem_free(T);

    free(parts);
    return (seq_t)sum_count - (seq_t)prod_count;
}
