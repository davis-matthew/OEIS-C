/*
 * A258765  Smallest m such that |A258057(m)| = n.
 * https://oeis.org/A258765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with |A003415(m+1) - A003415(m)| = n; computed
 *            by a segmented sieve of the arithmetic derivative A003415,
 *            scanning m upward and recording the first m for each
 *            |difference|
 * Range:     u8 n <= 9, u16 n <= 108, u32 n <= 177, u64 n <= 177,
 *            u128 n <= 177, big any n
 * Validated: standard, pass, 75 terms through n = 170 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A258765_MAXN 65536
#define A258765_BLOCK (1u << 18)
#define A258765_PLIM (1u << 18)
#define A258765_CAP ((uint64_t)1 << 36)

static long long A258765_first[A258765_MAXN];
static int A258765_init = 0;
static uint64_t A258765_next = 0;      /* next number whose derivative is to be computed */
static uint64_t A258765_prevd = 0;     /* derivative of A258765_next - 1 */
static int A258765_haveprev = 0;
static uint32_t *A258765_primes = NULL;
static size_t A258765_np = 0;
static uint64_t *A258765_rem = NULL;
static uint64_t *A258765_d = NULL;

static int A258765_setup(void) {
    for (int i = 0; i < A258765_MAXN; i++) A258765_first[i] = -1;
    unsigned char *comp = (unsigned char *)calloc(A258765_PLIM + 1, 1);
    A258765_primes = (uint32_t *)malloc(sizeof(uint32_t) * (A258765_PLIM / 2 + 16));
    A258765_rem = (uint64_t *)malloc(sizeof(uint64_t) * A258765_BLOCK);
    A258765_d = (uint64_t *)malloc(sizeof(uint64_t) * A258765_BLOCK);
    if (!comp || !A258765_primes || !A258765_rem || !A258765_d) { free(comp); return 0; }
    for (uint32_t i = 2; i <= A258765_PLIM; i++) {
        if (!comp[i]) {
            A258765_primes[A258765_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A258765_PLIM; j += i) comp[j] = 1;
        }
    }
    free(comp);
    return 1;
}

static void A258765_block(void) {
    uint64_t lo = A258765_next, hi = lo + A258765_BLOCK; /* [lo, hi) */
    uint64_t *rem = A258765_rem, *d = A258765_d;
    for (uint32_t i = 0; i < A258765_BLOCK; i++) { rem[i] = lo + i; d[i] = 0; }
    for (size_t t = 0; t < A258765_np; t++) {
        uint64_t p = A258765_primes[t];
        if (p * p >= hi) break;
        for (uint64_t pk = p; pk < hi; pk *= p) {
            uint64_t s = (lo + pk - 1) / pk * pk;
            if (s == 0) s = pk;
            for (uint64_t m = s; m < hi; m += pk) {
                uint64_t i = m - lo;
                d[i] += m / p;
                rem[i] /= p;
            }
            if (pk > hi / p) break;
        }
    }
    for (uint32_t i = 0; i < A258765_BLOCK; i++) {
        uint64_t m = lo + i;
        if (m >= 2 && rem[i] > 1) d[i] += m / rem[i];
        if (m < 2) d[i] = 0;
        if (A258765_haveprev) {
            uint64_t x = d[i], y = A258765_prevd;
            uint64_t ad = x > y ? x - y : y - x;
            if (ad < A258765_MAXN && A258765_first[ad] < 0)
                A258765_first[ad] = (long long)(m - 1);
        }
        A258765_prevd = d[i];
        A258765_haveprev = 1;
    }
    A258765_next = hi;
}

seq_t a(idx_t n) {
    if (!A258765_init) {
        if (!A258765_setup()) return (seq_t)0;
        A258765_init = 1;
    }
    if (n >= A258765_MAXN) return (seq_t)0;
    while (A258765_first[n] < 0 && A258765_next < A258765_CAP)
        A258765_block();
    if (A258765_first[n] < 0) return (seq_t)0;
    return (seq_t)(uint64_t)A258765_first[n];
}
