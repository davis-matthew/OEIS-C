/*
 * A351404  Decimal expansion of Sum_{k>=1} A106400(k-1)/k.
 * https://oeis.org/A351404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   constant C = Sum_{k>=1} A106400(k-1)/k = Integral_{u=0..inf}
 *            exp(-u)*Prod_{i>=0}(1-exp(-2^i*u)) du, evaluated by a trapezoid
 *            rule in v=log(u) with fixed-point big arithmetic; a(n) =
 *            (n+1)-th decimal digit of C after "0."
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define TM_NL 18
#define TM_FR 16
#define TM_MAXD 130

static void tm_mul(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint32_t w[2 * TM_NL];
    int i, j;
    for (i = 0; i < 2 * TM_NL; i++) w[i] = 0;
    for (i = 0; i < TM_NL; i++) {
        uint64_t carry = 0;
        if (!a[i]) continue;
        for (j = 0; j < TM_NL; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + w[i + j] + carry;
            w[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        w[i + TM_NL] = (uint32_t)carry;
    }
    for (i = 0; i < TM_NL; i++) r[i] = w[i + TM_FR];
}

static void tm_add(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < TM_NL; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        r[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void tm_sub(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    int64_t br = 0;
    int i;
    for (i = 0; i < TM_NL; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        r[i] = (uint32_t)t;
    }
}

static void tm_div_small(uint32_t *r, const uint32_t *a, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = TM_NL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a[i];
        r[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void tm_shr1(uint32_t *x)
{
    int i;
    for (i = 0; i < TM_NL; i++)
        x[i] = (x[i] >> 1) | (i + 1 < TM_NL ? (x[i + 1] << 31) : 0);
}

static bool tm_zero(const uint32_t *x)
{
    int i;
    for (i = 0; i < TM_NL; i++) if (x[i]) return false;
    return true;
}

static void tm_copy(uint32_t *r, const uint32_t *a)
{
    int i;
    for (i = 0; i < TM_NL; i++) r[i] = a[i];
}

static void tm_one(uint32_t *r)
{
    int i;
    for (i = 0; i < TM_NL; i++) r[i] = 0;
    r[TM_FR] = 1;
}

/* r = exp(+u) or exp(-u) */
static void tm_exp(uint32_t *r, const uint32_t *u, bool neg)
{
    uint32_t x[TM_NL], sum[TM_NL], term[TM_NL];
    int s = 0, n, i;
    tm_copy(x, u);
    while (x[TM_NL - 1] || x[TM_FR] || (x[TM_FR - 1] >> 24)) {
        tm_shr1(x);
        s++;
    }
    tm_one(sum);
    tm_one(term);
    for (n = 1; n < 400; n++) {
        tm_mul(term, term, x);
        tm_div_small(term, term, (uint32_t)n);
        if (tm_zero(term)) break;
        if (neg && (n & 1)) tm_sub(sum, sum, term);
        else tm_add(sum, sum, term);
    }
    for (i = 0; i < s; i++) tm_mul(sum, sum, sum);
    tm_copy(r, sum);
}

static void tm_point(uint32_t *acc, const uint32_t *u)
{
    uint32_t q[TM_NL], P[TM_NL], cur[TM_NL], f[TM_NL], one[TM_NL];
    int it;
    tm_one(one);
    tm_exp(q, u, true);
    tm_one(P);
    tm_copy(cur, q);
    for (it = 0; it < 90; it++) {
        if (tm_zero(cur)) break;
        tm_sub(f, one, cur);
        tm_mul(P, P, f);
        if (tm_zero(P)) return;
        tm_mul(cur, cur, cur);
    }
    tm_mul(P, P, q);
    tm_mul(P, P, u);
    tm_add(acc, acc, P);
}

static unsigned char tm_digits[TM_MAXD];
static bool tm_ready = false;

static void tm_compute(void)
{
    uint32_t acc[TM_NL], x[TM_NL], E[TM_NL], Einv[TM_NL], u[TM_NL];
    int k, i, d;
    for (i = 0; i < TM_NL; i++) { acc[i] = 0; x[i] = 0; }
    x[TM_FR - 1] = (uint32_t)1 << 26; /* 1/64 */
    tm_exp(E, x, false);
    tm_exp(Einv, x, true);

    tm_one(u);
    tm_point(acc, u);
    for (k = 1; k <= 372; k++) {
        tm_mul(u, u, E);
        tm_point(acc, u);
    }
    tm_one(u);
    for (k = 1; k <= 1400; k++) {
        tm_mul(u, u, Einv);
        tm_point(acc, u);
    }
    for (i = 0; i < 6; i++) tm_shr1(acc); /* times h = 1/64 */

    acc[TM_FR] = 0;
    acc[TM_FR + 1] = 0;
    for (d = 0; d < TM_MAXD; d++) {
        uint64_t carry = 0;
        for (i = 0; i < TM_FR; i++) {
            uint64_t t = (uint64_t)acc[i] * 10 + carry;
            acc[i] = (uint32_t)t;
            carry = t >> 32;
        }
        tm_digits[d] = (unsigned char)carry;
    }
    tm_ready = true;
}

seq_t a(idx_t n)
{
    if (!tm_ready) tm_compute();
    if (n >= TM_MAXD) return (seq_t)0;
    return (seq_t)tm_digits[(int)n];
}
