/*
 * A392510  Decimal expansion of the total number of possible books in the
 *          fictional library described in the short story "The Library of
 *          Babel" by J. L. Borges (1941).
 * https://oeis.org/A392510
 *
 * Offset:    1834098    (a(1834098) is the first term)
 * Keywords:  nonn, cons, fini, easy
 * Formula:   a(n) = decimal digit of 25^1312000 at position n-1834098
 * Range:     u8 n <= 1834199, u16 n <= 1834199, u32 n <= 1834199,
 *            u64 n <= 1834199, u128 n <= 1834199, big any n
 * Validated: partial, pass, 102 terms through n = 1834199 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number is 25^1312000 = 5^2624000, which has 1834098 digits; a(k) is
 * its digit k - 1834097 counted from the most significant (a(1834098) the
 * first). It is computed once, independently of seq_t, as 5^2624000 by
 * square-and-multiply on little-endian base-10^4 limbs: a square is a
 * polynomial product by Karatsuba on int64 coefficients (at most about 8e17,
 * so exact) followed by one carry pass. (The original raised 5 to that power
 * in seq_t, which only big can hold, and printed it into a 64-byte buffer.) */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define EXPONENT 2624000u
#define NDIGITS 1834098u
#define FIRST 1834098u                      /* the index of the first digit */
#define LB 10000                            /* limb base */

/* r[0..2n) = a[0..n) * b[0..n), n a power of 2; t: scratch of 4n */
static void kara(const int64_t *a, const int64_t *b, size_t n, int64_t *r, int64_t *t)
{
    if (n <= 32) {
        memset(r, 0, 2 * n * sizeof *r);
        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j) r[i + j] += a[i] * b[j];
        return;
    }
    size_t h = n / 2;
    int64_t *sa = t, *sb = t + h, *z1 = t + n, *rest = t + 2 * n;
    kara(a, b, h, r, rest);                 /* a0 b0 -> r[0, n) */
    kara(a + h, b + h, h, r + n, rest);     /* a1 b1 -> r[n, 2n) */
    for (size_t i = 0; i < h; ++i) {
        sa[i] = a[i] + a[i + h];
        sb[i] = b[i] + b[i + h];
    }
    kara(sa, sb, h, z1, rest);              /* (a0 + a1)(b0 + b1) */
    for (size_t i = 0; i < n; ++i) z1[i] -= r[i] + r[n + i];
    for (size_t i = 0; i < n; ++i) r[h + i] += z1[i];
}

static int64_t *X = NULL;                   /* the number, len limbs */
static size_t len = 0;

static void carry(int64_t *v, size_t *n)    /* normalize to limbs < LB */
{
    int64_t c = 0;
    for (size_t i = 0; i < *n; ++i) {
        int64_t s = v[i] + c;
        v[i] = s % LB;
        c = s / LB;
    }
    while (c) { v[(*n)++] = c % LB; c /= LB; }
    while (*n > 1 && v[*n - 1] == 0) --*n;
}

static unsigned char *digits = NULL;        /* most significant first */

static void build(void)
{
    size_t room = 1;
    while (room < NDIGITS / 4 + 8) room *= 2;
    X = (int64_t *)calloc(2 * room + 8, sizeof *X);
    int64_t *A = (int64_t *)calloc(room, sizeof *A);
    int64_t *R = (int64_t *)calloc(2 * room + 8, sizeof *R);
    int64_t *T = (int64_t *)calloc(4 * room, sizeof *T);
    if (!X || !A || !R || !T) abort();
    X[0] = 1;
    len = 1;
    int top = 31;
    while (!((EXPONENT >> top) & 1)) --top;
    for (int bit = top; bit >= 0; --bit) {
        size_t n = 1;                       /* square X */
        while (n < len) n *= 2;
        memcpy(A, X, len * sizeof *A);
        memset(A + len, 0, (n - len) * sizeof *A);
        kara(A, A, n, R, T);
        size_t m = 2 * n;
        carry(R, &m);
        memcpy(X, R, m * sizeof *X);
        len = m;
        if ((EXPONENT >> bit) & 1) {        /* times 5 */
            for (size_t i = 0; i < len; ++i) X[i] *= 5;
            carry(X, &len);
        }
    }
    free(A);
    free(R);
    free(T);
    digits = (unsigned char *)malloc(len * 4);
    if (!digits) abort();
    size_t k = 0;
    for (size_t i = len; i-- > 0;) {        /* top limb without its leading zeros */
        int64_t v = X[i];
        unsigned char d[4];
        for (int j = 3; j >= 0; --j) { d[j] = (unsigned char)(v % 10); v /= 10; }
        int j = 0;
        if (i == len - 1) while (j < 3 && d[j] == 0) ++j;
        for (; j < 4; ++j) digits[k++] = d[j];
    }
    free(X);
    if (k != NDIGITS) abort();
}

seq_t a(idx_t n)
{
    if (n < FIRST || n - FIRST >= NDIGITS) return (seq_t)0;
    if (!digits) build();
    return (seq_t)digits[n - FIRST];
}
