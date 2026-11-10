/*
 * A345051  Numbers k such that A345048(k) is equal to A345049(k).
 * https://oeis.org/A345051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th number k with A342001(k)*A051709(k) =
 *            A173557(k)*A345001(k). * Dividing by k/rad(k) gives
 *            k'*(sigma+phi-2k) = phi*(sigma+k'-2k), * i.e. (k' -
 *            phi(k))*(sigma(k) - 2k) = 0: k is perfect (A000396) or * k' =
 *            phi(k) (A166374). Search: k' = phi(k) <=> Sum e_i/p_i = Prod
 *            (1-1/p_i) * over p_i...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A345051_T = NULL;
static size_t A345051_Tn = 0, A345051_Tcap = 0;
static uint64_t A345051_X = 0;
static uint32_t *A345051_P = NULL;
static size_t A345051_Pn = 0;

static void A345051_push(uint64_t v)
{
    if (A345051_Tn == A345051_Tcap) {
        A345051_Tcap = A345051_Tcap ? 2 * A345051_Tcap : 64;
        A345051_T = (uint64_t *)realloc(A345051_T, A345051_Tcap * sizeof(uint64_t));
    }
    A345051_T[A345051_Tn++] = v;
}

/* returns 1 and *out = m*b^f if <= X, else 0 */
static int A345051_powle(uint64_t m, uint64_t b, int f, uint64_t X, uint64_t *out)
{
    uint64_t v = m;
    for (int i = 0; i < f; ++i) {
        if (v > X / b) return 0;
        v *= b;
    }
    *out = v;
    return 1;
}

static void A345051_dfs(uint64_t m, uint64_t maxp, size_t i0,
                        uint64_t L, uint64_t A, uint64_t B)
{
    uint64_t X = A345051_X;
    uint64_t D = A - B;              /* > 0 guaranteed */

    /* close with the largest prime q^f */
    for (int f = 1; f <= 64; ++f) {
        uint64_t num = A + (uint64_t)f * L;
        uint64_t lb = num / D;
        if (lb < maxp + 1) lb = maxp + 1;
        uint64_t v;
        if (!A345051_powle(m, lb, f, X, &v)) break;
        if (num % D == 0) {
            uint64_t q = num / D;
            if (q > maxp && is_prime_u64(q)) {
                if (A345051_powle(m, q, f, X, &v)) A345051_push(v);
            }
        }
    }

    /* extend by an intermediate odd prime p > maxp */
    for (size_t i = i0; i < A345051_Pn; ++i) {
        uint64_t p = A345051_P[i];
        if (p <= maxp) continue;
        if (m > X / p / p) break;
        uint64_t Lp = L * p;
        uint64_t Ap = A * (p - 1);
        uint64_t mm = m;
        for (uint64_t e = 1;; ++e) {
            mm *= p;
            uint64_t Bp = B * p + e * L;
            if (Bp >= Ap) break;
            A345051_dfs(mm, p, i + 1, Lp, Ap, Bp);
            if (mm > X / p / p) break;
        }
    }
}

static int A345051_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static void A345051_build(uint64_t X)
{
    A345051_X = X;
    A345051_Tn = 0;

    /* odd primes up to sqrt(X) */
    uint64_t lim = 1;
    while ((lim + 1) <= X / (lim + 1)) ++lim;
    lim += 2;
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A345051_P);
    A345051_P = (uint32_t *)malloc(((size_t)lim / 2 + 2) * sizeof(uint32_t));
    A345051_Pn = 0;
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (!c[i]) {
            A345051_P[A345051_Pn++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += 2 * i) c[j] = 1;
        }
    }
    free(c);

    A345051_dfs(1, 1, 0, 1, 1, 0);

    /* perfect numbers */
    static const int mp[] = {2, 3, 5, 7, 13, 17, 19, 31};
    for (int i = 0; i < 8; ++i) {
        uint64_t v = ((uint64_t)1 << (mp[i] - 1)) * (((uint64_t)1 << mp[i]) - 1);
        if (v <= X) A345051_push(v);
    }
    qsort(A345051_T, A345051_Tn, sizeof(uint64_t), A345051_cmp);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A345051_X == 0) A345051_build((uint64_t)1 << 16);
    while (A345051_Tn < n && A345051_X < ((uint64_t)1 << 56))
        A345051_build(A345051_X * 4);
    if (A345051_Tn < n) return (seq_t)0;
    return (seq_t)A345051_T[n - 1];
}
