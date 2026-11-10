/*
 * A326141  Odd numbers n for which A318879(n) is not zero and A318879(n)
 *          divides A318878(n); odd numbers such that A326140(n) = A318879(n).
 * https://oeis.org/A326141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th odd k with A318879(k) > 0 and A318879(k) |
 *            A318878(k); computed by a segmented sieve over odd k (phi(k)/k <
 *            1/2 prefilter), then exact divisor sums
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A326141_terms = 0;
static size_t A326141_cnt = 0, A326141_cap = 0;
static uint64_t A326141_next = 1;          /* next odd number to scan */
static uint64_t *A326141_pr = 0;
static size_t A326141_np = 0, A326141_pcap = 0;

static void A326141_addprime_upto(uint64_t lim)
{
    if (A326141_np == 0) {
        A326141_pcap = 1024;
        A326141_pr = (uint64_t *)malloc(A326141_pcap * sizeof(uint64_t));
        A326141_pr[A326141_np++] = 3;
    }
    uint64_t c = A326141_pr[A326141_np - 1];
    while (A326141_pr[A326141_np - 1] * A326141_pr[A326141_np - 1] <= lim) {
        for (;;) {
            c += 2;
            int ok = 1;
            for (size_t i = 0; i < A326141_np && A326141_pr[i] * A326141_pr[i] <= c; i++)
                if (c % A326141_pr[i] == 0) { ok = 0; break; }
            if (ok) break;
        }
        if (A326141_np == A326141_pcap) {
            A326141_pcap *= 2;
            A326141_pr = (uint64_t *)realloc(A326141_pr, A326141_pcap * sizeof(uint64_t));
        }
        A326141_pr[A326141_np++] = c;
    }
}

static int A326141_test(uint64_t n)
{
    static uint64_t D[16384], P[16384];
    size_t nd = 1;
    D[0] = 1; P[0] = 1;
    uint64_t m = n;
    for (size_t i = 0; i < A326141_np && A326141_pr[i] * A326141_pr[i] <= m; i++) {
        uint64_t p = A326141_pr[i];
        if (m % p) continue;
        size_t base = nd;
        uint64_t pw = 1, ph = 1;
        while (m % p == 0) {
            m /= p;
            pw *= p;
            ph = (pw == p) ? p - 1 : ph * p;
            for (size_t j = 0; j < base; j++) {
                D[nd] = D[j] * pw; P[nd] = P[j] * ph; nd++;
            }
        }
    }
    if (m > 1) {
        size_t base = nd;
        for (size_t j = 0; j < base; j++) {
            D[nd] = D[j] * m; P[nd] = P[j] * (m - 1); nd++;
        }
    }
    unsigned __int128 s1 = 0, s2 = 0;
    for (size_t j = 0; j < nd; j++) {
        uint64_t d = D[j], t = 2 * P[j];
        if (d > t) s1 += d - t;
        else if (t > d) s2 += t - d;
    }
    return s1 != 0 && s2 % s1 == 0;
}

seq_t a(idx_t n)
{
    const uint64_t S = 1u << 16;
    static uint64_t *rem = 0, *phi = 0;
    if (n == 0) return 0;
    if (!rem) {
        rem = (uint64_t *)malloc(S * sizeof(uint64_t));
        phi = (uint64_t *)malloc(S * sizeof(uint64_t));
    }
    while (A326141_cnt < (size_t)n) {
        uint64_t lo = A326141_next;
        uint64_t hi = lo + 2 * (S - 1);
        A326141_addprime_upto(hi);
        for (uint64_t i = 0; i < S; i++) { rem[i] = lo + 2 * i; phi[i] = 1; }
        for (size_t k = 0; k < A326141_np; k++) {
            uint64_t p = A326141_pr[k];
            if (p * p > hi) break;
            uint64_t m = ((lo + p - 1) / p) * p;
            if ((m & 1) == 0) m += p;
            for (; m <= hi; m += 2 * p) {
                uint64_t i = (m - lo) >> 1;
                uint64_t r = rem[i] / p;
                uint64_t ph = p - 1;
                while (r % p == 0) { r /= p; ph *= p; }
                rem[i] = r;
                phi[i] *= ph;
            }
        }
        for (uint64_t i = 0; i < S; i++) {
            uint64_t v = lo + 2 * i;
            uint64_t ph = phi[i];
            if (rem[i] > 1) ph *= rem[i] - 1;
            if (2 * ph < v && A326141_test(v)) {
                if (A326141_cnt == A326141_cap) {
                    A326141_cap = A326141_cap ? A326141_cap * 2 : 256;
                    A326141_terms = (uint64_t *)realloc(A326141_terms, A326141_cap * sizeof(uint64_t));
                }
                A326141_terms[A326141_cnt++] = v;
            }
        }
        A326141_next = hi + 2;
    }
    return (seq_t)A326141_terms[n - 1];
}
