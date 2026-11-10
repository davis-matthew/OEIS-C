/*
 * A269738  Index in A092243 where -n first appears.
 * https://oeis.org/A269738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search. Sieve the primes with a segmented mod-30 wheel sieve and
 *            track the A092243 score: a(1) = 0, and each later step adds
 *            sign(gap(k) - gap(k-1)), where gap(k) = prime(k+1) - prime(k).
 *            a(n) is the first k at which the score equals -n
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8812 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define A269738_SEGB ((uint64_t)1 << 19)          /* bytes per segment (30 numbers per byte) */
#define A269738_BASE_LIMIT ((uint32_t)1 << 22)    /* base primes cover numbers up to ~1.7e13 */

static const uint8_t A269738_R[8] = {1, 7, 11, 13, 17, 19, 23, 29};
static int8_t A269738_bitpos[30];

static uint64_t *A269738_rec = NULL;
static size_t A269738_len = 0, A269738_cap = 0;

static uint32_t *A269738_bp = NULL;     /* base primes >= 7 */
static uint64_t *A269738_nxt = NULL;    /* 8 next absolute byte indices per prime */
static uint8_t *A269738_mk = NULL;      /* 8 bit masks per prime */
static size_t A269738_nb = 0, A269738_act = 0;
static uint8_t *A269738_seg = NULL;

static bool A269738_init_done = false;
static uint64_t A269738_curB = 0;
static long long A269738_score = 0;
static uint64_t A269738_prev = 0, A269738_prevgap = 0, A269738_k = 0;

static void A269738_push(uint64_t k)
{
    if (A269738_len == A269738_cap) {
        A269738_cap = A269738_cap ? A269738_cap * 2 : 1024;
        A269738_rec = (uint64_t *)realloc(A269738_rec, A269738_cap * sizeof(uint64_t));
        if (!A269738_rec) abort();
    }
    A269738_rec[A269738_len++] = k;
}

static void A269738_init(void)
{
    uint32_t L = A269738_BASE_LIMIT;
    unsigned char *c = (unsigned char *)calloc((size_t)L + 1, 1);
    if (!c) abort();
    for (uint32_t i = 2; (uint64_t)i * i <= L; ++i)
        if (!c[i])
            for (uint32_t j = i * i; j <= L; j += i) c[j] = 1;
    size_t cnt = 0;
    for (uint32_t i = 7; i <= L; i += 2) if (!c[i]) ++cnt;
    A269738_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A269738_nxt = (uint64_t *)malloc(cnt * 8 * sizeof(uint64_t));
    A269738_mk = (uint8_t *)malloc(cnt * 8);
    A269738_seg = (uint8_t *)malloc((size_t)A269738_SEGB);
    if (!A269738_bp || !A269738_nxt || !A269738_mk || !A269738_seg) abort();
    cnt = 0;
    for (uint32_t i = 7; i <= L; i += 2) if (!c[i]) A269738_bp[cnt++] = i;
    A269738_nb = cnt;
    free(c);
    for (int i = 0; i < 30; ++i) A269738_bitpos[i] = -1;
    for (int j = 0; j < 8; ++j) A269738_bitpos[A269738_R[j]] = (int8_t)j;

    /* primes 2,3,5: a(1)=0 at k=1 (gap 3-2=1); k=2: gap 5-3=2 > 1 -> score 1 */
    A269738_push(1);
    A269738_k = 2;
    A269738_score = 1;
    A269738_prev = 5;
    A269738_prevgap = 2;
    A269738_curB = 0;
    A269738_init_done = true;
}

static void A269738_segment(void)
{
    uint64_t loB = A269738_curB, hiB = loB + A269738_SEGB;
    uint64_t hiN = hiB * 30;
    uint8_t *s = A269738_seg;
    memset(s, 0, (size_t)A269738_SEGB);
    if (loB == 0) s[0] |= 1; /* the number 1 */

    while (A269738_act < A269738_nb) {
        uint64_t p = A269738_bp[A269738_act];
        if (p * p >= hiN) break;
        size_t base = A269738_act * 8;
        uint64_t pm = p % 30;
        for (int j = 0; j < 8; ++j) {
            uint64_t q0 = p + (((uint64_t)A269738_R[j] + 30 - pm) % 30);
            uint64_t m = p * q0;
            A269738_nxt[base + j] = m / 30;
            A269738_mk[base + j] = (uint8_t)(1u << A269738_bitpos[m % 30]);
        }
        ++A269738_act;
    }
    for (size_t i = 0; i < A269738_act; ++i) {
        uint64_t p = A269738_bp[i];
        size_t base = i * 8;
        for (int j = 0; j < 8; ++j) {
            uint64_t o = A269738_nxt[base + j];
            uint8_t m = A269738_mk[base + j];
            for (; o < hiB; o += p) s[o - loB] |= m;
            A269738_nxt[base + j] = o;
        }
    }

    long long score = A269738_score;
    uint64_t prev = A269738_prev, prevgap = A269738_prevgap, k = A269738_k;
    for (uint64_t i = 0; i < A269738_SEGB; ++i) {
        unsigned w = (unsigned)(uint8_t)~s[i];
        if (!w) continue;
        uint64_t b30 = (loB + i) * 30;
        while (w) {
            int b = __builtin_ctz(w);
            w &= w - 1;
            uint64_t p = b30 + A269738_R[b];
            if (p < 7) continue;
            uint64_t g = p - prev;
            ++k;
            if (g > prevgap) score++;
            else if (g < prevgap) {
                score--;
                if (score < 0 && (uint64_t)(-score) == (uint64_t)A269738_len)
                    A269738_push(k);
            }
            prev = p;
            prevgap = g;
        }
    }
    A269738_score = score;
    A269738_prev = prev;
    A269738_prevgap = prevgap;
    A269738_k = k;
    A269738_curB = hiB;
}

seq_t a(idx_t n)
{
    if (!A269738_init_done) A269738_init();
    while ((uint64_t)A269738_len <= (uint64_t)n) A269738_segment();
    return (seq_t)A269738_rec[(size_t)n];
}
