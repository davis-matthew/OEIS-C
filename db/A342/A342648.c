/*
 * A342648  Composite numbers k such that k-A238525(k) and k+A238525(k) are
 *          prime.
 * https://oeis.org/A342648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k composite, r = k mod sopfr(k) (A238525), keep k when
 *            k-r and k+r are both prime; sopfr by a segmented sieve,
 *            primality by a bit sieve
 * Range:     u8 n <= 5, u16 n <= 98, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9365 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint8_t *A342648_comp = 0;   /* bit set => composite (or 0/1) */
static uint64_t A342648_N = 0;
static uint64_t *A342648_terms = 0;
static size_t A342648_cnt = 0, A342648_cap = 0;
static uint64_t A342648_done = 3;   /* all k <= done have been examined */

static bool A342648_isp(uint64_t x)
{
    return !((A342648_comp[x >> 3] >> (x & 7)) & 1);
}

static void A342648_sieve(uint64_t N)
{
    free(A342648_comp);
    A342648_comp = (uint8_t *)calloc((size_t)(N / 8 + 1), 1);
    A342648_N = N;
    A342648_comp[0] |= 3; /* 0 and 1 */
    for (uint64_t i = 2; i * i <= N; i++) {
        if (A342648_isp(i)) {
            for (uint64_t j = i * i; j <= N; j += i)
                A342648_comp[j >> 3] |= (uint8_t)(1u << (j & 7));
        }
    }
}

static void A342648_push(uint64_t k)
{
    if (A342648_cnt == A342648_cap) {
        A342648_cap = A342648_cap ? A342648_cap * 2 : 1024;
        A342648_terms = (uint64_t *)realloc(A342648_terms, A342648_cap * sizeof(uint64_t));
    }
    A342648_terms[A342648_cnt++] = k;
}

static void A342648_extend(uint64_t limit)
{
    A342648_sieve(2 * limit + 2);

    /* primes up to sqrt(limit) */
    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= limit) sq++;
    size_t np = 0;
    uint32_t *pr = (uint32_t *)malloc((size_t)(sq + 2) * sizeof(uint32_t));
    for (uint64_t p = 2; p <= sq; p++)
        if (A342648_isp(p)) pr[np++] = (uint32_t)p;

    const uint64_t B = 1u << 16;
    uint64_t *rem = (uint64_t *)malloc((size_t)B * sizeof(uint64_t));
    uint64_t *sum = (uint64_t *)malloc((size_t)B * sizeof(uint64_t));

    for (uint64_t lo = A342648_done + 1; lo <= limit; lo += B) {
        uint64_t hi = lo + B - 1;
        if (hi > limit) hi = limit;
        uint64_t len = hi - lo + 1;
        for (uint64_t i = 0; i < len; i++) { rem[i] = lo + i; sum[i] = 0; }
        for (size_t t = 0; t < np; t++) {
            uint64_t p = pr[t];
            if (p * p > hi) break;
            uint64_t st = ((lo + p - 1) / p) * p;
            for (uint64_t j = st; j <= hi; j += p) {
                uint64_t i = j - lo;
                do { rem[i] /= p; sum[i] += p; } while (rem[i] % p == 0);
            }
        }
        for (uint64_t i = 0; i < len; i++) {
            uint64_t k = lo + i;
            if (A342648_isp(k)) continue;
            uint64_t s = sum[i] + (rem[i] > 1 ? rem[i] : 0);
            uint64_t r = k % s;
            if (r == 0) continue;
            if (A342648_isp(k - r) && A342648_isp(k + r))
                A342648_push(k);
        }
    }
    A342648_done = limit;
    free(rem);
    free(sum);
    free(pr);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t limit = A342648_done < (1u << 16) ? (1u << 16) : A342648_done;
    while (A342648_cnt < (size_t)n) {
        if (limit <= A342648_done) limit = A342648_done * 2;
        A342648_extend(limit);
    }
    return (seq_t)A342648_terms[n - 1];
}
