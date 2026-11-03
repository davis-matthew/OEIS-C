/*
 * A275984  Decimal expansion of 2^44497 - 1, the 27th Mersenne prime
 *          A000668(27).
 * https://oeis.org/A275984
 *
 * Offset:    13395    (a(13395) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = the n-th decimal digit of 2^44497 – 1
 * Range:     u8 n <= 26789, u16 n <= 26789, u32 n <= 26789, u64 n <= 26789,
 *            u128 n <= 26789, big any n
 * Validated: full, pass, 13395 terms through n = 26789 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A275984: the decimal digits of 2^44497 - 1, which has 13395 of them; the
 * offset is 13395, so a(13395) is the leading digit.
 *
 * A digit far down needs the whole number: it is built exactly in base 10^9,
 * multiplying by 2^29 at a time. The leading digits come much faster from
 * 2^44497 computed by squaring with only the top LEAD_LIMBS limbs kept: each
 * truncation is off by under one unit in the last of the limbs, and the 50 or
 * so steps leave the first LEAD_DIGITS digits exact. (Subtracting 1 changes
 * only the last digit: 2^k ends in 2, 4, 6 or 8.) Digits are small, so this is
 * the same at every width. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define EXPONENT 44497
#define DIGITS 13395                   /* = the offset */
#define LEAD_LIMBS 340                    /* 3060 digits */
#define LEAD_DIGITS 3000

static unsigned char *all = NULL;         /* every digit, once built */
static unsigned char lead[LEAD_DIGITS];
static int lead_built = 0;

static void build_all(void)
{
    const size_t LIMBS = DIGITS / 9 + 2;
    uint32_t *L = (uint32_t *)calloc(LIMBS, sizeof *L);   /* little-endian base 10^9 */
    all = (unsigned char *)malloc(DIGITS + 9);
    if (!L || !all) abort();
    size_t len = 1;
    L[0] = 1;
    for (int left = EXPONENT; left > 0;) {
        int s = left < 29 ? left : 29;
        left -= s;
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t v = ((uint64_t)L[j] << s) + carry;
            L[j] = (uint32_t)(v % 1000000000u);
            carry = v / 1000000000u;
        }
        while (carry) {
            L[len++] = (uint32_t)(carry % 1000000000u);
            carry /= 1000000000u;
        }
    }
    L[0] -= 1;
    size_t pos = 0;
    for (size_t j = len; j-- > 0;) {
        char buf[9];
        uint32_t v = L[j];
        for (int d = 0; d < 9; ++d) { buf[d] = (char)(v % 10); v /= 10; }
        int top = 8;
        if (j == len - 1) while (top > 0 && buf[top] == 0) --top;
        for (int d = top; d >= 0; --d) all[pos++] = (unsigned char)buf[d];
    }
    free(L);
}

/* r = the top LEAD_LIMBS limbs of x*y (big-endian base 10^9, leading limb nonzero) */
static void mul_top(const uint32_t *x, int nx, const uint32_t *y, int ny, uint32_t *r, int *nr)
{
    uint64_t prod[2 * LEAD_LIMBS + 1];
    int n = nx + ny;
    memset(prod, 0, sizeof prod);
    for (int i = nx - 1; i >= 0; --i)                 /* little-endian accumulation */
        for (int j = ny - 1; j >= 0; --j) {
            int k = (nx - 1 - i) + (ny - 1 - j);
            prod[k] += (uint64_t)x[i] * y[j];
            prod[k + 1] += prod[k] / 1000000000u;
            prod[k] %= 1000000000u;
        }
    for (int k = 0; k < n; ++k) {
        prod[k + 1] += prod[k] / 1000000000u;
        prod[k] %= 1000000000u;
    }
    while (n > 1 && prod[n - 1] == 0) --n;
    int keep = n < LEAD_LIMBS ? n : LEAD_LIMBS;
    for (int i = 0; i < keep; ++i) r[i] = (uint32_t)prod[n - 1 - i];
    *nr = keep;
}

static void build_lead(void)
{
    uint32_t res[LEAD_LIMBS] = {1}, base[LEAD_LIMBS] = {2}, t[LEAD_LIMBS];
    int nres = 1, nbase = 1, nt;
    for (unsigned e = EXPONENT; e; e >>= 1) {
        if (e & 1) {
            mul_top(res, nres, base, nbase, t, &nt);
            memcpy(res, t, sizeof t);
            nres = nt;
        }
        if (e > 1) {
            mul_top(base, nbase, base, nbase, t, &nt);
            memcpy(base, t, sizeof t);
            nbase = nt;
        }
    }
    int pos = 0;
    for (int i = 0; i < nres && pos < LEAD_DIGITS; ++i) {
        char buf[9];
        uint32_t v = res[i];
        for (int d = 0; d < 9; ++d) { buf[d] = (char)(v % 10); v /= 10; }
        int top = 8;
        if (i == 0) while (top > 0 && buf[top] == 0) --top;
        for (int d = top; d >= 0 && pos < LEAD_DIGITS; --d) lead[pos++] = (unsigned char)buf[d];
    }
    lead_built = 1;
}

seq_t a(idx_t n)
{
    if (n < DIGITS || n >= 2 * (idx_t)DIGITS) return (seq_t)0;
    size_t i = (size_t)(n - DIGITS);              /* position from the left */
    if (DIGITS > 100000 && i < LEAD_DIGITS) {
        if (!lead_built) build_lead();
        return (seq_t)lead[i];
    }
    if (!all) build_all();
    return (seq_t)all[i];
}
