/*
 * A343532  Primes in A343531.
 * https://oeis.org/A343532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among the terms of A343531, where A343531(m) =
 *            Sum_{i=1..m} (prime(i+1)-prime(i))*prime(m+1-i); computed by
 *            scanning m upward and testing primality
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 1012, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Validated: standard, pass, 59 terms through n = 4315 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint32_t *A343532_pr = NULL;
static size_t A343532_cnt = 0;
static uint64_t *A343532_found = NULL;
static size_t A343532_nf = 0, A343532_cf = 0;
static uint64_t A343532_m = 0;

static void A343532_sieve(size_t need)
{
    size_t limit = A343532_cnt ? (size_t)A343532_pr[A343532_cnt - 1] * 2 : 1000;
    for (;;) {
        char *s = (char *)calloc(limit + 1, 1);
        if (!s) abort();
        size_t c = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!s[i]) {
                ++c;
                for (size_t j = i * i; j <= limit; j += i) s[j] = 1;
            }
        }
        if (c >= need) {
            free(A343532_pr);
            A343532_pr = (uint32_t *)malloc(c * sizeof(uint32_t));
            if (!A343532_pr) abort();
            size_t k = 0;
            for (size_t i = 2; i <= limit; ++i) if (!s[i]) A343532_pr[k++] = (uint32_t)i;
            A343532_cnt = c;
            free(s);
            return;
        }
        free(s);
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    while (A343532_nf < (size_t)n) {
        uint64_t m = ++A343532_m;
        if (A343532_cnt < m + 1) A343532_sieve((size_t)(m + 1) * 2);
        const uint32_t *p = A343532_pr;
        uint64_t sum = 0;
        for (uint64_t i = 0; i < m; ++i)
            sum += (uint64_t)(p[i + 1] - p[i]) * p[m - 1 - i];
        if (sum == 2 || ((sum & 1) && is_prime_u64(sum))) {
            if (A343532_nf == A343532_cf) {
                A343532_cf = A343532_cf ? A343532_cf * 2 : 64;
                A343532_found = (uint64_t *)realloc(A343532_found, A343532_cf * sizeof(uint64_t));
                if (!A343532_found) abort();
            }
            A343532_found[A343532_nf++] = sum;
        }
    }
    return (seq_t)A343532_found[n - 1];
}
