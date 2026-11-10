/*
 * A282783  Number of terms in A282430 less than or equal to 10^n.
 * https://oeis.org/A282783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   sieve search. e = 6m+2 is uncovered iff 6m+1 and every e - 3^k
 *            (k >= 1) are composite. e = 6m+4 (e > 4) is uncovered iff every
 *            e - 3^k (k >= 1) is composite. The program counts these e <=
 *            10^n. Primes 6m+1 and 6m+5 are sieved in segments into two
 *            circular bitsets. Each bitset spans only the large...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

#define A282783_NSP 16
static const unsigned A282783_sp[A282783_NSP] =
    {5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};

static void A282783_sieve_seg(uint64_t *D, uint64_t w0, uint64_t S,
                              uint64_t *const *mk, const unsigned *r0,
                              const uint64_t *bp, uint64_t *nx, size_t nbp)
{
    for (uint64_t t = 0; t < S; t++) D[t] = ~(uint64_t)0;
    for (int s = 0; s < A282783_NSP; s++) {
        uint64_t p = A282783_sp[s];
        uint64_t x = (r0[s] + p - (64 * (w0 % p)) % p) % p;
        uint64_t step = p - (64 % p);
        const uint64_t *m = mk[s];
        for (uint64_t t = 0; t < S; t++) {
            D[t] &= ~m[x];
            x += step; if (x >= p) x -= p;
        }
    }
    uint64_t base = w0 * 64, hb = (w0 + S) * 64;
    for (size_t j = 0; j < nbp; j++) {
        uint64_t i = nx[j], p = bp[j];
        for (; i < hb; i += p) {
            uint64_t o = i - base;
            D[o >> 6] &= ~((uint64_t)1 << (o & 63));
        }
        nx[j] = i;
    }
}

static void A282783_stream(const uint64_t *buf, uint64_t R, uint64_t OFF,
                           uint64_t w0, uint64_t S, uint64_t sh, uint64_t *acc)
{
    uint64_t qw = sh >> 6;
    unsigned rb = (unsigned)(sh & 63);
    uint64_t ih = (w0 + OFF - qw) % R;
    if (rb == 0) {
        for (uint64_t t = 0; t < S; t++) {
            acc[t] |= buf[ih];
            if (++ih == R) ih = 0;
        }
    } else {
        uint64_t il = ih ? ih - 1 : R - 1;
        for (uint64_t t = 0; t < S; t++) {
            acc[t] |= (buf[il] >> (64 - rb)) | (buf[ih] << rb);
            il = ih;
            if (++ih == R) ih = 0;
        }
    }
}

static uint64_t A282783_count(uint64_t N)
{
    if (N < 10) return N >= 2 ? 1 : 0;
    const uint64_t S = 4096;
    uint64_t MA = (N - 2) / 6, MB = (N - 4) / 6;
    uint64_t W = MA / 64 + 1;
    uint64_t nseg = (W + S - 1) / S;
    uint64_t Wend = nseg * S;

    uint64_t shA[64], shB[64], maxsh = 0;
    int ns = 0;
    for (uint64_t pw = 3; pw < N; ) {
        uint64_t j = (pw - 3) / 6;
        shA[ns] = j + 1; shB[ns] = j; ns++;
        if (j + 1 > maxsh) maxsh = j + 1;
        if (pw > N / 3) break;
        pw *= 3;
    }
    uint64_t maxqw = maxsh >> 6;
    uint64_t OFF = ((maxqw + 2 + S - 1) / S) * S;
    uint64_t R = OFF + S;

    uint64_t *buf1 = (uint64_t *)calloc((size_t)R, sizeof(uint64_t));
    uint64_t *buf5 = (uint64_t *)calloc((size_t)R, sizeof(uint64_t));
    uint64_t *accA = (uint64_t *)malloc((size_t)S * sizeof(uint64_t));
    uint64_t *accB = (uint64_t *)malloc((size_t)S * sizeof(uint64_t));

    /* base primes */
    uint64_t vmax = 6 * (Wend * 64) + 5;
    uint64_t lim = isqrt_u64(vmax) + 1;
    unsigned char *bs = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t *bp  = (uint64_t *)malloc(((size_t)lim / 2 + 2) * sizeof(uint64_t));
    uint64_t *nx1 = (uint64_t *)malloc(((size_t)lim / 2 + 2) * sizeof(uint64_t));
    uint64_t *nx5 = (uint64_t *)malloc(((size_t)lim / 2 + 2) * sizeof(uint64_t));
    size_t nbp = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (bs[i]) continue;
        for (uint64_t j = i * i; j <= lim; j += i) bs[j] = 1;
        if (i > 61) {
            bp[nbp] = i;
            nx1[nbp] = (i * i - 1) / 6;
            uint64_t q = i + ((i % 6 == 1) ? 4 : 2);
            nx5[nbp] = (i * q - 5) / 6;
            nbp++;
        }
    }
    free(bs);

    uint64_t *mk[A282783_NSP];
    unsigned r1[A282783_NSP], r5[A282783_NSP];
    for (int s = 0; s < A282783_NSP; s++) {
        unsigned p = A282783_sp[s];
        mk[s] = (uint64_t *)calloc(p, sizeof(uint64_t));
        for (unsigned j = 0; j < 64; j++) mk[s][j % p] |= (uint64_t)1 << j;
        for (unsigned t = 1; t < 6; t++) {
            unsigned v = t * p;
            if (v % 6 == 1) r1[s] = (v - 1) / 6;
            if (v % 6 == 5) r5[s] = (v - 5) / 6;
        }
    }

    uint64_t count = 0;
    for (uint64_t sg = 0; sg < nseg; sg++) {
        uint64_t w0 = sg * S;
        uint64_t d = (w0 + OFF) % R;
        uint64_t *D1 = buf1 + d, *D5 = buf5 + d;
        A282783_sieve_seg(D1, w0, S, mk, r1, bp, nx1, nbp);
        A282783_sieve_seg(D5, w0, S, mk, r5, bp, nx5, nbp);
        if (w0 == 0) {
            D1[0] &= ~(uint64_t)1;                  /* 1 is not prime */
            for (int s = 0; s < A282783_NSP; s++) {
                unsigned p = A282783_sp[s], c = p % 6;
                unsigned m = (p - c) / 6;
                if (c == 1) D1[0] |= (uint64_t)1 << m;
                else        D5[0] |= (uint64_t)1 << m;
            }
        }
        for (uint64_t t = 0; t < S; t++) { accA[t] = D1[t]; accB[t] = 0; }
        for (int k = 0; k < ns; k++) {
            A282783_stream(buf5, R, OFF, w0, S, shA[k], accA);
            A282783_stream(buf1, R, OFF, w0, S, shB[k], accB);
        }
        for (uint64_t t = 0; t < S; t++) {
            uint64_t w = w0 + t, lo = 64 * w;
            if (lo <= MA) {
                uint64_t va = ~accA[t];
                if (lo + 63 > MA) va &= ((uint64_t)1 << (MA - lo + 1)) - 1;
                count += (uint64_t)__builtin_popcountll(va);
            }
            if (lo <= MB) {
                uint64_t vb = ~accB[t];
                if (lo + 63 > MB) vb &= ((uint64_t)1 << (MB - lo + 1)) - 1;
                if (w == 0) vb &= ~(uint64_t)1;     /* e = 4 = 3 + 1 */
                count += (uint64_t)__builtin_popcountll(vb);
            }
        }
    }

    for (int s = 0; s < A282783_NSP; s++) free(mk[s]);
    free(bp); free(nx1); free(nx5);
    free(buf1); free(buf5); free(accA); free(accB);
    return count;
}

seq_t a(idx_t n)
{
    static uint64_t res[20];
    static bool have[20];
    if (n == 0) return (seq_t)0;
    if (n >= 20) return (seq_t)0;
    if (!have[n]) {
        uint64_t N = 1;
        for (idx_t i = 0; i < n; i++) N *= 10;
        res[n] = A282783_count(N);
        have[n] = true;
    }
    return (seq_t)res[n];
}
