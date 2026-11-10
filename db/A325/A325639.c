/*
 * A325639  Numbers n for which A091255(n, sigma(n)) = n.
 * https://oeis.org/A325639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. a(n) is the n-th number k with sigma(k) = A048720(k, j)
 *            for some j, i.e. sigma(k) is divisible by k as a GF(2)[X]
 *            polynomial. Write k = 2^e*m with m odd. A segmented sieve finds
 *            sigma(m) for odd m. Then for each e <= v2(sigma(m)) it tests
 *            (2^(e+1)-1)*sigma(m) / 2^e for GF(2) divisibility...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define A325639_S 65536

static uint64_t *A325639_terms = NULL;
static size_t A325639_cnt = 0, A325639_cap = 0;
static int A325639_dirty = 0;
static uint64_t A325639_next = 1;          /* next odd m to process */

static uint32_t *A325639_pr = NULL;
static size_t A325639_np = 0;
static uint64_t A325639_plim = 0;

static uint64_t A325639_sg[A325639_S];
static uint64_t A325639_P[A325639_S];

static void A325639_primes(uint64_t L)
{
    if (A325639_plim >= L) return;
    uint64_t lim = A325639_plim ? A325639_plim : 65536;
    while (lim < L) lim *= 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A325639_pr);
    A325639_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    A325639_np = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            A325639_pr[A325639_np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A325639_plim = lim;
}

static int A325639_bl(uint64_t x)
{
    return 64 - __builtin_clzll(x);
}

/* does d divide s as GF(2) polynomials? (d > 0) */
static int A325639_gfdiv(uint64_t d, uint64_t s)
{
    int dd = A325639_bl(d);
    while (s) {
        int ds = A325639_bl(s);
        if (ds < dd) return 0;
        s ^= d << (ds - dd);
    }
    return 1;
}

static void A325639_add(uint64_t v)
{
    if (A325639_cnt == A325639_cap) {
        A325639_cap = A325639_cap ? 2 * A325639_cap : 256;
        A325639_terms = (uint64_t *)realloc(A325639_terms, sizeof(uint64_t) * A325639_cap);
    }
    A325639_terms[A325639_cnt++] = v;
    A325639_dirty = 1;
}

static void A325639_segment(void)
{
    uint64_t lo = A325639_next;                       /* odd */
    uint64_t hi = lo + 2 * (uint64_t)(A325639_S - 1); /* inclusive, odd */
    size_t i;

    uint64_t sq = 1;
    while ((sq + 1) * (sq + 1) <= hi) sq++;
    A325639_primes(sq + 1);

    for (i = 0; i < A325639_S; i++) {
        A325639_sg[i] = 1;
        A325639_P[i] = 1;
    }

    for (size_t k = 1; k < A325639_np; k++) {
        uint64_t p = A325639_pr[k];
        if (p * p > hi) break;
        uint64_t t = (lo + p - 1) / p;
        if (!(t & 1)) t++;
        uint64_t m0 = t * p;
        if (m0 > hi) continue;
        size_t idx = (size_t)((m0 - lo) / 2);
        uint64_t c = t % p;
        for (; idx < A325639_S; idx += (size_t)p) {
            if (c == 0) {
                uint64_t mm = (lo + 2 * (uint64_t)idx) / p / p;
                uint64_t pw = p * p, term = 1 + p + p * p;
                while (mm % p == 0) {
                    mm /= p;
                    pw *= p;
                    term += pw;
                }
                A325639_sg[idx] *= term;
                A325639_P[idx] *= pw;
            } else {
                A325639_sg[idx] *= (p + 1);
                A325639_P[idx] *= p;
            }
            c += 2;
            if (c >= p) c -= p;
        }
    }

    for (i = 0; i < A325639_S; i++) {
        uint64_t m = lo + 2 * (uint64_t)i;
        uint64_t r;
        if (m < 4294967296ULL)
            r = (uint64_t)((uint32_t)m / (uint32_t)A325639_P[i]);
        else
            r = m / A325639_P[i];
        uint64_t s = A325639_sg[i];
        if (r > 1) s *= (r + 1);
        int v = __builtin_ctzll(s);
        for (int e = 0; e <= v; e++) {
            if (m > ((1ULL << 61) >> e)) break;
            /* sigma(2^e m) / 2^e = ((2^(e+1)-1) * s) >> e */
            unsigned __int128 w = ((unsigned __int128)s << (e + 1)) - s;
            uint64_t q = (uint64_t)(w >> e);
            if (A325639_gfdiv(m, q)) A325639_add(m << e);
        }
    }
    A325639_next = hi + 2;
}

static void A325639_sort(void)
{
    for (size_t i = 1; i < A325639_cnt; i++) {
        uint64_t x = A325639_terms[i];
        size_t j = i;
        while (j > 0 && A325639_terms[j - 1] > x) {
            A325639_terms[j] = A325639_terms[j - 1];
            j--;
        }
        A325639_terms[j] = x;
    }
    A325639_dirty = 0;
}

seq_t a(idx_t n)
{
    for (;;) {
        if (A325639_dirty) A325639_sort();
        uint64_t X = 2 * A325639_next - 1;   /* all terms <= X are found */
        size_t c = 0;
        while (c < A325639_cnt && A325639_terms[c] <= X) c++;
        if ((uint64_t)c >= (uint64_t)n) return (seq_t)A325639_terms[n - 1];
        A325639_segment();
    }
}
