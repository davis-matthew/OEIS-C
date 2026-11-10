/*
 * A359451  Decimal expansion of Sum_{k>=1} 1/A359450(k).
 * https://oeis.org/A359451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, base
 * Formula:   Let S = Sum_{k>=1} 1/A359450(k) (A359450(1)=1, A359450(2)=2,
 *            A359450(n)=n*A359450(A070939(n))). S = (5/3 - 3*log(2)/2 +
 *            Sum_{m>=3} (H(2^m-1)-H(2^(m-1)-1)-log(2))/A359450(m)) /
 *            (1-log(2)). a(n) = n-th decimal digit of S (a(1)=2), via
 *            multiprecision fixed point arithmetic: direct summation for
 *            smal...
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: partial, pass, 101 terms through n = 101 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define FX_W 24
#define FX_L (FX_W + 2)
#define FX_DIGITS 190
#define FX_M0 19

typedef struct { uint32_t w[FX_L]; } fx_t;

static void fx_zero(fx_t *a) { int i; for (i = 0; i < FX_L; i++) a->w[i] = 0; }
static int fx_iszero(const fx_t *a) { int i; for (i = 0; i < FX_L; i++) if (a->w[i]) return 0; return 1; }
static void fx_add(fx_t *a, const fx_t *b) {
    uint64_t c = 0; int i;
    for (i = 0; i < FX_L; i++) { uint64_t cur = (uint64_t)a->w[i] + b->w[i] + c; a->w[i] = (uint32_t)cur; c = cur >> 32; }
}
static void fx_sub(fx_t *a, const fx_t *b) {
    int64_t br = 0; int i;
    for (i = 0; i < FX_L; i++) {
        int64_t cur = (int64_t)a->w[i] - (int64_t)b->w[i] - br;
        if (cur < 0) { cur += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->w[i] = (uint32_t)cur;
    }
}
static void fx_mul64(fx_t *a, uint64_t m) {
    unsigned __int128 c = 0; int i;
    for (i = 0; i < FX_L; i++) {
        unsigned __int128 cur = (unsigned __int128)a->w[i] * m + c;
        a->w[i] = (uint32_t)cur; c = cur >> 32;
    }
}
static void fx_div(fx_t *a, uint32_t d) {
    uint64_t rem = 0; int i;
    for (i = FX_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a->w[i];
        a->w[i] = (uint32_t)(cur / d); rem = cur % d;
    }
}
static void fx_shr(fx_t *a, unsigned bits) {
    unsigned ws = bits / 32, bs = bits % 32; int i;
    fx_t r;
    if (ws >= FX_L) { fx_zero(a); return; }
    for (i = 0; i < FX_L; i++) {
        uint64_t lo = (i + (int)ws < FX_L) ? a->w[i + ws] : 0;
        uint64_t hi = (i + (int)ws + 1 < FX_L) ? a->w[i + ws + 1] : 0;
        r.w[i] = bs ? (uint32_t)((lo >> bs) | (hi << (32 - bs))) : (uint32_t)lo;
    }
    *a = r;
}
static void fx_mul(fx_t *r, const fx_t *a, const fx_t *b) {
    uint32_t t[2 * FX_L]; int i, j;
    for (i = 0; i < 2 * FX_L; i++) t[i] = 0;
    for (i = 0; i < FX_L; i++) {
        uint64_t carry = 0;
        for (j = 0; j < FX_L; j++) {
            uint64_t cur = (uint64_t)t[i + j] + (uint64_t)a->w[i] * b->w[j] + carry;
            t[i + j] = (uint32_t)cur; carry = cur >> 32;
        }
        t[i + FX_L] = (uint32_t)carry;
    }
    for (i = 0; i < FX_L; i++) r->w[i] = t[i + FX_W];
}

static uint64_t fx_A359450(uint64_t n) {
    uint64_t b = 0, x = n;
    if (n == 1) return 1;
    if (n == 2) return 2;
    while (x) { b++; x >>= 1; }
    return n * fx_A359450(b);
}

static int fx_ready = 0;
static unsigned char fx_digits[FX_DIGITS];

static void fx_compute(void) {
    static const uint64_t bnum[17] = {0, 1, 1, 1, 1, 5, 691, 7, 3617, 43867, 174611, 854513,
        236364091ULL, 8553103ULL, 23749461029ULL, 8615841276005ULL, 7709321041217ULL};
    static const uint32_t bden[17] = {1, 6, 30, 42, 30, 66, 2730, 6, 510, 798, 330, 138,
        2730, 6, 870, 14322, 510};
    /* index k: B_{2k}; for k=17: 2577687858367/6 handled separately */
    fx_t ln2, pw, term, R, sum, P, Q, T, c, S, tmp, nx;
    int k, m, it, i;
    unsigned maxm = 32 * FX_W + 2;

    fx_zero(&ln2); fx_zero(&pw); pw.w[FX_W - 1] = 0x80000000u;
    for (k = 1; k <= (int)maxm; k++) {
        term = pw; fx_div(&term, (uint32_t)k); fx_add(&ln2, &term); fx_shr(&pw, 1);
    }
    fx_zero(&R);
    for (m = 3; m < FX_M0; m++) {
        uint32_t j, lo = (uint32_t)1 << (m - 1), hi = ((uint32_t)1 << m) - 1;
        fx_zero(&sum);
        for (j = lo; j <= hi; j++) {
            uint64_t rem = 1; fx_zero(&T);
            for (i = FX_W - 1; i >= 0; i--) {
                uint64_t cur = rem << 32;
                T.w[i] = (uint32_t)(cur / j); rem = cur % j;
            }
            fx_add(&sum, &T);
        }
        fx_sub(&sum, &ln2);
        fx_div(&sum, (uint32_t)fx_A359450((uint64_t)m));
        fx_add(&R, &sum);
    }
    for (m = FX_M0; m <= (int)maxm; m++) {
        fx_zero(&P); P.w[FX_W] = 1; fx_shr(&P, (unsigned)m + 1);
        fx_zero(&Q);
        for (k = 1; k <= 17; k++) {
            uint64_t num = (k == 17) ? 2577687858367ULL : bnum[k];
            uint32_t den = (k == 17) ? 6 : bden[k];
            T = (fx_t){{0}};
            T.w[FX_W] = 1;
            fx_shr(&T, (unsigned)(2 * k * m));
            if (fx_iszero(&T)) break;
            fx_div(&T, (uint32_t)(2 * k));
            fx_div(&T, den);
            fx_mul64(&T, num);
            fx_mul64(&T, ((uint64_t)1 << (2 * k)) - 1);
            if (k & 1) fx_add(&P, &T); else fx_add(&Q, &T);
        }
        fx_sub(&P, &Q);
        fx_div(&P, (uint32_t)fx_A359450((uint64_t)m));
        fx_add(&R, &P);
    }
    /* c = 5/3 - 3/2 ln2 + R */
    fx_zero(&c); c.w[FX_W] = 5; fx_div(&c, 3);
    tmp = ln2; fx_mul64(&tmp, 3); fx_div(&tmp, 2);
    fx_sub(&c, &tmp); fx_add(&c, &R);
    S = c;
    for (it = 0; it < 3000; it++) {
        fx_mul(&tmp, &ln2, &S);
        nx = c; fx_add(&nx, &tmp);
        {
            int same = 1;
            for (i = 0; i < FX_L; i++) if (nx.w[i] != S.w[i]) { same = 0; break; }
            S = nx;
            if (same) break;
        }
    }
    fx_digits[0] = (unsigned char)S.w[FX_W];
    S.w[FX_W] = 0; S.w[FX_W + 1] = 0;
    for (k = 1; k < FX_DIGITS; k++) {
        fx_mul64(&S, 10);
        fx_digits[k] = (unsigned char)S.w[FX_W];
        S.w[FX_W] = 0; S.w[FX_W + 1] = 0;
    }
    fx_ready = 1;
}

seq_t a(idx_t n) {
    if (n < 1 || n > FX_DIGITS) return (seq_t)0;
    if (!fx_ready) fx_compute();
    return (seq_t)fx_digits[(size_t)(n - 1)];
}
