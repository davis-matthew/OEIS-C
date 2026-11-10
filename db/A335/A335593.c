/*
 * A335593  Numbers k such that abs(A335592(k))/2 is prime.
 * https://oeis.org/A335593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that abs(A335592(k))/2 is prime;
 *            A335592(k)=p1*p4-p3*p2 with p1,p2,p3,p4 the k-th primes ending
 *            in 1,3,7,9. Computed by sieve + scan
 * Range:     u8 n <= 16, u16 n <= 2644, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9089 (2026-11-08)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

static uint64_t *A335593_found = NULL;
static size_t A335593_nfound = 0;
static uint64_t A335593_limit = 0;

static void A335593_extend(void)
{
    uint64_t L = A335593_limit ? A335593_limit * 2 : 65536;
    uint64_t *pr = NULL;
    size_t cnt = 0;
    build_primes_upto((idx_t)L, &pr, &cnt);
    uint64_t *c[4];
    size_t m[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        c[i] = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    for (size_t i = 0; i < cnt; i++) {
        switch ((unsigned)(pr[i] % 10)) {
            case 1: c[0][m[0]++] = pr[i]; break;
            case 3: c[1][m[1]++] = pr[i]; break;
            case 7: c[2][m[2]++] = pr[i]; break;
            case 9: c[3][m[3]++] = pr[i]; break;
        }
    }
    size_t K = m[0];
    for (int i = 1; i < 4; i++) if (m[i] < K) K = m[i];
    free(A335593_found);
    A335593_found = (uint64_t *)malloc((K + 1) * sizeof(uint64_t));
    A335593_nfound = 0;
    for (size_t k = 0; k < K; k++) {
        uint64_t x = c[0][k] * c[3][k];
        uint64_t y = c[1][k] * c[2][k];
        uint64_t d = x > y ? x - y : y - x;
        if ((d & 1) == 0 && d >= 4 && is_prime_u64(d / 2))
            A335593_found[A335593_nfound++] = (uint64_t)k + 1;
    }
    for (int i = 0; i < 4; i++) free(c[i]);
    free(pr);
    A335593_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A335593_nfound < n)
        A335593_extend();
    return (seq_t)A335593_found[n - 1];
}
