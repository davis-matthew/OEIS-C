/*
 * A348605  Odd nonexponential abundant numbers: odd numbers k such that
 *          A160135(k) > k.
 * https://oeis.org/A348605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A160135(k) > k, where A160135(k) =
 *            sigma(k) - A051377(k) (sum of non-exponential divisors);
 *            computed by a segmented sieve over odd k
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9972 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A348605_terms = NULL;
static size_t A348605_cnt = 0, A348605_cap = 0;
static uint64_t A348605_next = 1;          /* next odd start of block */
static uint32_t *A348605_pr = NULL;
static size_t A348605_npr = 0;
static uint64_t A348605_prlim = 0;

static void A348605_build_primes(uint64_t lim)
{
    if (lim <= A348605_prlim) return;
    uint64_t L = lim * 2 + 100;
    char *s = (char *)calloc((size_t)L + 1, 1);
    free(A348605_pr);
    A348605_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)L / 2 + 10));
    A348605_npr = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!s[i]) {
            A348605_pr[A348605_npr++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
    A348605_prlim = L;
}

static void A348605_block(void)
{
    const uint64_t B = 1u << 16;
    uint64_t lo = A348605_next;               /* odd */
    uint64_t hi = lo + B;
    uint64_t sq = 1;
    while (sq * sq <= hi) sq++;
    A348605_build_primes(sq + 1);

    uint64_t *rem = (uint64_t *)malloc(sizeof(uint64_t) * B);
    uint64_t *sg = (uint64_t *)malloc(sizeof(uint64_t) * B);
    uint64_t *es = (uint64_t *)malloc(sizeof(uint64_t) * B);
    for (uint64_t i = 0; i < B; i++) { rem[i] = lo + i; sg[i] = 1; es[i] = 1; }

    for (size_t pi = 0; pi < A348605_npr; pi++) {
        uint64_t p = A348605_pr[pi];
        if (p == 2) continue;
        if (p > sq) break;
        uint64_t m = ((lo + p - 1) / p) * p;
        if ((m & 1) == 0) m += p;
        for (; m < hi; m += 2 * p) {
            uint64_t i = m - lo;
            uint64_t e = 0, r = rem[i];
            uint64_t pw = 1, ssum = 1;
            while (r % p == 0) {
                r /= p; e++;
                pw *= p; ssum += pw;
            }
            rem[i] = r;
            sg[i] *= ssum;
            uint64_t t = 0;
            for (uint64_t d = 1; d <= e; d++)
                if (e % d == 0) {
                    uint64_t q = 1;
                    for (uint64_t j = 0; j < d; j++) q *= p;
                    t += q;
                }
            es[i] *= t;
        }
    }
    for (uint64_t i = 0; i < B; i += 2) {
        uint64_t k = lo + i;
        uint64_t s = sg[i], e = es[i];
        if (rem[i] > 1) { s *= rem[i] + 1; e *= rem[i]; }
        if (s - e > k) {
            if (A348605_cnt == A348605_cap) {
                A348605_cap = A348605_cap ? A348605_cap * 2 : 1024;
                A348605_terms = (uint64_t *)realloc(A348605_terms, A348605_cap * sizeof(uint64_t));
            }
            A348605_terms[A348605_cnt++] = k;
        }
    }
    free(rem); free(sg); free(es);
    A348605_next = lo + B;
}

seq_t a(idx_t n)
{
    while (A348605_cnt < (size_t)n) A348605_block();
    return (seq_t)A348605_terms[n - 1];
}
