/*
 * A352587  Even numbers 2m such that A352612(2m) = A103131(2m).
 * https://oeis.org/A352587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over even N = 2m. Since the product of the totatives of N
 *            is a unit mod N, A352612(N) == A103131(N) (mod N) reduces to
 *            prod{p < N/2, p and N-p prime} p*(N-p) == 1 (mod N); a(n) is the
 *            n-th such N
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *Q_comp = NULL;   /* Q_comp[i] != 0 => i not prime */
static uint64_t Q_lim = 0;
static uint32_t *Q_primes = NULL;      /* odd primes up to Q_lim */
static size_t Q_np = 0;
static uint64_t *Q_terms = NULL;
static size_t Q_cnt = 0, Q_cap = 0;
static uint64_t Q_next = 2;

static void Q_sieve(uint64_t L)
{
    if (L <= Q_lim) return;
    uint64_t nl = Q_lim ? Q_lim : 65536;
    while (nl < L) nl *= 2;
    free(Q_comp);
    free(Q_primes);
    Q_comp = (unsigned char *)calloc((size_t)nl + 1, 1);
    Q_comp[0] = 1;
    Q_comp[1] = 1;
    for (uint64_t i = 4; i <= nl; i += 2) Q_comp[i] = 1;
    for (uint64_t i = 3; i * i <= nl; i += 2)
        if (!Q_comp[i])
            for (uint64_t j = i * i; j <= nl; j += 2 * i) Q_comp[j] = 1;
    size_t cnt = 0;
    for (uint64_t i = 3; i <= nl; i += 2) if (!Q_comp[i]) cnt++;
    Q_primes = (uint32_t *)malloc((cnt + 1) * sizeof(uint32_t));
    Q_np = 0;
    for (uint64_t i = 3; i <= nl; i += 2) if (!Q_comp[i]) Q_primes[Q_np++] = (uint32_t)i;
    Q_lim = nl;
}

/* N even: test prod over Goldbach pairs p < q, p + q = N, of p*q == 1 mod N */
static bool Q_test(uint64_t N)
{
    uint64_t prod = 1 % N;
    uint64_t half = N / 2;
    for (size_t i = 0; i < Q_np; i++) {
        uint64_t p = Q_primes[i];
        if (p >= half) break;            /* p == N/2 shares a factor with N */
        if (Q_comp[N - p]) continue;
        uint64_t t = (p * (N - p)) % N;
        prod = (prod * t) % N;
    }
    return prod == 1 % N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (Q_cnt < (size_t)n) {
        uint64_t N = Q_next;
        Q_sieve(N + 2);
        if (Q_test(N)) {
            if (Q_cnt == Q_cap) {
                Q_cap = Q_cap ? Q_cap * 2 : 128;
                Q_terms = (uint64_t *)realloc(Q_terms, Q_cap * sizeof(uint64_t));
            }
            Q_terms[Q_cnt++] = N;
        }
        Q_next += 2;
    }
    return (seq_t)Q_terms[n - 1];
}
