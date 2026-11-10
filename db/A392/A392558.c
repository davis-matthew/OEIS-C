/*
 * A392558  Decimal expansion of the number whose continued fraction
 *          coefficients are given in A390946.
 * https://oeis.org/A392558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   x = [0; c(1), c(2), ...] where c = A390946 (CFs of
 *            prime(i)/prime(j+1)); a(n) = n-th decimal digit of x after "0."
 *            (a(0) is the first), computed from a big-integer convergent P/Q
 *            and long division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9506 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *A392558_digs = NULL;
static uint64_t A392558_dlen = 0;

static void A392558_compute(uint64_t D)
{
    uint64_t target_bits = (uint64_t)((D + 40) * 3.33) + 64;
    size_t need = (size_t)(target_bits / 32 + 2);
    size_t cap = need + 4;
    uint32_t *P1 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *P2 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *Q1 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *Q2 = (uint32_t *)calloc(cap, sizeof(uint32_t));
    if (!P1 || !P2 || !Q1 || !Q2) abort();
    /* P1 = p_{k-1}, P2 = p_{k-2}; start: p_0=0, p_{-1}=1; q_0=1, q_{-1}=0 */
    P1[0] = 0; P2[0] = 1;
    Q1[0] = 1; Q2[0] = 0;
    size_t len = 1;

    size_t pcap = 64, pn = 0;
    uint32_t *pr = (uint32_t *)malloc(pcap * sizeof(uint32_t));
    if (!pr) abort();
    pr[pn++] = 2;
    size_t d = 1, i = 0;
    bool done = false;

    while (!done) {
        while (pn <= d) {
            uint32_t c = pr[pn - 1] + 1;
            for (;; ++c) {
                bool ok = true;
                for (size_t k = 0; k < pn; ++k) {
                    uint64_t p2 = (uint64_t)pr[k] * pr[k];
                    if (p2 > c) break;
                    if (c % pr[k] == 0) { ok = false; break; }
                }
                if (ok) break;
            }
            if (pn == pcap) {
                pcap *= 2;
                pr = (uint32_t *)realloc(pr, pcap * sizeof(uint32_t));
                if (!pr) abort();
            }
            pr[pn++] = c;
        }
        uint64_t num = pr[i], den = pr[d];
        while (num > 0 && !done) {
            uint64_t q = den / num, r = den % num;
            /* apply term q */
            uint64_t cp = 0, cq = 0;
            for (size_t k = 0; k < len; ++k) {
                uint64_t t = q * P1[k] + P2[k] + cp;
                P2[k] = (uint32_t)t; cp = t >> 32;
                uint64_t u = q * Q1[k] + Q2[k] + cq;
                Q2[k] = (uint32_t)u; cq = u >> 32;
            }
            if (cp || cq) {
                P2[len] = (uint32_t)cp;
                Q2[len] = (uint32_t)cq;
                len++;
            }
            uint32_t *t;
            t = P1; P1 = P2; P2 = t;
            t = Q1; Q1 = Q2; Q2 = t;
            den = num; num = r;
            if (len > need) done = true;
        }
        ++i;
        if (i == d) { i = 0; ++d; }
    }

    /* long division P1 / Q1 */
    size_t rl = len + 2;
    uint32_t *R = (uint32_t *)calloc(rl, sizeof(uint32_t));
    if (!R) abort();
    for (size_t k = 0; k < len; ++k) R[k] = P1[k];
    free(A392558_digs);
    A392558_digs = (unsigned char *)malloc((size_t)D + 1);
    if (!A392558_digs) abort();
    for (uint64_t pos = 0; pos < D; ++pos) {
        uint64_t carry = 0;
        for (size_t k = 0; k < rl; ++k) {
            uint64_t t = (uint64_t)R[k] * 10 + carry;
            R[k] = (uint32_t)t; carry = t >> 32;
        }
        unsigned dg = 0;
        for (;;) {
            /* compare R >= Q1 */
            int ge = 1;
            for (size_t k = rl; k-- > 0;) {
                uint32_t qv = (k < len) ? Q1[k] : 0;
                if (R[k] != qv) { ge = R[k] > qv; break; }
            }
            if (!ge) break;
            int64_t borrow = 0;
            for (size_t k = 0; k < rl; ++k) {
                int64_t t = (int64_t)R[k] - (k < len ? (int64_t)Q1[k] : 0) - borrow;
                if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
                R[k] = (uint32_t)t;
            }
            dg++;
        }
        A392558_digs[pos] = (unsigned char)dg;
    }
    A392558_dlen = D;
    free(R); free(P1); free(P2); free(Q1); free(Q2); free(pr);
}

seq_t a(idx_t n)
{
    if ((uint64_t)n >= A392558_dlen) {
        uint64_t D = (uint64_t)n + 1;
        if (D < 2 * A392558_dlen) D = 2 * A392558_dlen;
        if (D < 100) D = 100;
        A392558_compute(D);
    }
    return (seq_t)A392558_digs[n];
}
