/*
 * A392601  Numbers k such that A322582(k) <= A276085(k) <= A348507(k).
 * https://oeis.org/A392601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A322582(k) <= A276085(k) <= A348507(k),
 *            where A322582(k) = k - A003958(k), A348507(k) = A003959(k) - k,
 *            A276085 = fully additive with a(p) = (product of primes < p).
 *            Computed by a segmented sieve search, cached
 * Range:     u8 n <= 11, u16 n <= 288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9886 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A392601_CAP ((uint64_t)1 << 62)
#define A392601_SEG ((uint64_t)1 << 16)

static uint64_t *A392601_found = NULL;
static size_t A392601_cnt = 0, A392601_capn = 0;
static uint64_t A392601_done = 0; /* all k <= done examined */

static uint64_t *A392601_pr = NULL;
static uint64_t *A392601_pre = NULL;
static size_t A392601_np = 0, A392601_pcap = 0;

static uint64_t A392601_smul(uint64_t a, uint64_t b)
{
    if (a == 0 || b == 0) return 0;
    if (b > A392601_CAP / a) return A392601_CAP;
    uint64_t r = a * b;
    return r > A392601_CAP ? A392601_CAP : r;
}

static uint64_t A392601_sadd(uint64_t a, uint64_t b)
{
    uint64_t r = a + b;
    return r > A392601_CAP ? A392601_CAP : r;
}

static void A392601_push_prime(uint64_t p)
{
    if (A392601_np == A392601_pcap) {
        A392601_pcap = A392601_pcap ? A392601_pcap * 2 : 64;
        A392601_pr = (uint64_t *)realloc(A392601_pr, A392601_pcap * sizeof(uint64_t));
        A392601_pre = (uint64_t *)realloc(A392601_pre, A392601_pcap * sizeof(uint64_t));
    }
    A392601_pr[A392601_np] = p;
    A392601_pre[A392601_np] = A392601_np == 0 ? 1 :
        A392601_smul(A392601_pre[A392601_np - 1], A392601_pr[A392601_np - 1]);
    A392601_np++;
}

static void A392601_ensure_primes(uint64_t limit)
{
    if (A392601_np == 0) {
        A392601_push_prime(2);
        A392601_push_prime(3);
    }
    while (A392601_pr[A392601_np - 1] < limit) {
        uint64_t c = A392601_pr[A392601_np - 1] + 2;
        for (;; c += 2) {
            int ok = 1;
            for (size_t i = 1; i < A392601_np && A392601_pr[i] * A392601_pr[i] <= c; i++)
                if (c % A392601_pr[i] == 0) { ok = 0; break; }
            if (ok) break;
        }
        A392601_push_prime(c);
    }
}

static uint64_t A392601_pre_of(uint64_t P)
{
    size_t lo = 0, hi = A392601_np;
    if (P > A392601_pr[A392601_np - 1]) return A392601_CAP;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A392601_pr[mid] == P) return A392601_pre[mid];
        if (A392601_pr[mid] < P) lo = mid + 1; else hi = mid;
    }
    return A392601_CAP;
}

static void A392601_segment(void)
{
    uint64_t lo = A392601_done + 1, hi = lo + A392601_SEG; /* [lo, hi) */
    uint64_t s = 1;
    while (s * s <= hi) s++;
    A392601_ensure_primes(s > 200 ? s : 200);

    uint64_t *rem = (uint64_t *)malloc(A392601_SEG * sizeof(uint64_t));
    uint64_t *phi = (uint64_t *)malloc(A392601_SEG * sizeof(uint64_t));
    uint64_t *psi = (uint64_t *)malloc(A392601_SEG * sizeof(uint64_t));
    uint64_t *der = (uint64_t *)malloc(A392601_SEG * sizeof(uint64_t));
    for (uint64_t i = 0; i < A392601_SEG; i++) {
        rem[i] = lo + i; phi[i] = 1; psi[i] = 1; der[i] = 0;
    }
    for (size_t j = 0; j < A392601_np; j++) {
        uint64_t p = A392601_pr[j];
        if (p * p >= hi) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t m = start; m < hi; m += p) {
            uint64_t k = m - lo;
            uint64_t e = 0;
            while (rem[k] % p == 0) {
                rem[k] /= p;
                e++;
                phi[k] *= (p - 1);
                psi[k] *= (p + 1);
                der[k] = A392601_sadd(der[k], A392601_pre[j]);
            }
        }
    }
    for (uint64_t i = 0; i < A392601_SEG; i++) {
        uint64_t m = lo + i;
        if (rem[i] > 1) {
            uint64_t P = rem[i];
            phi[i] *= (P - 1);
            psi[i] *= (P + 1);
            der[i] = A392601_sadd(der[i], A392601_pre_of(P));
        }
        uint64_t L = m - phi[i];
        uint64_t U = psi[i] - m;
        if (L <= der[i] && der[i] <= U) {
            if (A392601_cnt == A392601_capn) {
                A392601_capn = A392601_capn ? A392601_capn * 2 : 1024;
                A392601_found = (uint64_t *)realloc(A392601_found, A392601_capn * sizeof(uint64_t));
            }
            A392601_found[A392601_cnt++] = m;
        }
    }
    free(rem); free(phi); free(psi); free(der);
    A392601_done = hi - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A392601_cnt < (size_t)n)
        A392601_segment();
    return (seq_t)A392601_found[n - 1];
}
