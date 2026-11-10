/*
 * A360508  Numbers k such that A300570(k) considered simply as a decimal
 *          string is prime.
 * https://oeis.org/A360508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more, changed
 * Formula:   a(n) = n-th k such that the decimal string formed by
 *            concatenating binary(k), binary(k-1), ..., binary(1)
 *            (A300570(k)) is prime; computed by incremental search with trial
 *            division + base-2 Fermat/Montgomery test on big integers
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned __int128 a360508_u128;

static uint64_t *a360508_terms = NULL;
static size_t a360508_cnt = 0, a360508_cap = 0;
static uint64_t a360508_k = 0;

static uint32_t *a360508_pr = NULL;
static size_t a360508_npr = 0;

static void a360508_init_primes(void)
{
    const uint32_t LIM = 50000;
    unsigned char *c;
    uint32_t i, j;
    if (a360508_pr) return;
    c = (unsigned char *)calloc(LIM + 1, 1);
    a360508_pr = (uint32_t *)malloc(sizeof(uint32_t) * 6000);
    for (i = 2; i <= LIM; i++) {
        if (!c[i]) {
            a360508_pr[a360508_npr++] = i;
            for (j = i * 2; j <= LIM; j += i) c[j] = 1;
        }
    }
    free(c);
}

static bool a360508_is_prime(uint64_t m, const uint64_t *N, size_t L);

/* Montgomery multiplication, CIOS */
static void a360508_mmul(uint64_t *r, const uint64_t *a, const uint64_t *b,
                         const uint64_t *N, uint64_t ninv, size_t L, uint64_t *t)
{
    size_t i, j;
    memset(t, 0, sizeof(uint64_t) * (L + 2));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0, m;
        a360508_u128 s;
        for (j = 0; j < L; j++) {
            s = (a360508_u128)t[j] + (a360508_u128)a[j] * b[i] + carry;
            t[j] = (uint64_t)s;
            carry = (uint64_t)(s >> 64);
        }
        s = (a360508_u128)t[L] + carry;
        t[L] = (uint64_t)s;
        t[L + 1] = (uint64_t)(s >> 64);
        m = t[0] * ninv;
        s = (a360508_u128)t[0] + (a360508_u128)m * N[0];
        carry = (uint64_t)(s >> 64);
        for (j = 1; j < L; j++) {
            s = (a360508_u128)t[j] + (a360508_u128)m * N[j] + carry;
            t[j - 1] = (uint64_t)s;
            carry = (uint64_t)(s >> 64);
        }
        s = (a360508_u128)t[L] + carry;
        t[L - 1] = (uint64_t)s;
        t[L] = t[L + 1] + (uint64_t)(s >> 64);
    }
    {
        bool sub = (t[L] != 0);
        if (!sub) {
            for (i = L; i-- > 0;) {
                if (t[i] != N[i]) { sub = t[i] > N[i]; break; }
                if (i == 0) sub = true;
            }
        }
        if (sub) {
            uint64_t borrow = 0;
            for (i = 0; i < L; i++) {
                uint64_t x = t[i], y = N[i];
                uint64_t d = x - y - borrow;
                borrow = (x < y) || (x == y && borrow) ? 1 : 0;
                t[i] = d;
            }
        }
        memcpy(r, t, sizeof(uint64_t) * L);
    }
}

/* x = 2x mod N, x < N */
static void a360508_dbl(uint64_t *x, const uint64_t *N, size_t L)
{
    uint64_t carry = 0;
    size_t i;
    bool sub;
    for (i = 0; i < L; i++) {
        uint64_t nc = x[i] >> 63;
        x[i] = (x[i] << 1) | carry;
        carry = nc;
    }
    sub = carry != 0;
    if (!sub) {
        for (i = L; i-- > 0;) {
            if (x[i] != N[i]) { sub = x[i] > N[i]; break; }
            if (i == 0) sub = true;
        }
    }
    if (sub) {
        uint64_t borrow = 0;
        for (i = 0; i < L; i++) {
            uint64_t a = x[i], b = N[i];
            uint64_t d = a - b - borrow;
            borrow = (a < b) || (a == b && borrow) ? 1 : 0;
            x[i] = d;
        }
    }
}

/* base-2 Fermat test for odd N > 1 */
static bool a360508_fermat2(const uint64_t *N, size_t L)
{
    uint64_t inv = 1, ninv;
    uint64_t *one, *x, *t;
    size_t i, bits;
    int b;
    bool ok;
    for (i = 0; i < 7; i++) inv *= 2 - N[0] * inv;
    ninv = (uint64_t)0 - inv;
    one = (uint64_t *)malloc(sizeof(uint64_t) * L);
    x = (uint64_t *)malloc(sizeof(uint64_t) * L);
    t = (uint64_t *)malloc(sizeof(uint64_t) * (L + 2));
    /* one = 2^(64L) mod N */
    memset(one, 0, sizeof(uint64_t) * L);
    one[0] = 1;
    for (bits = 0; bits < 64 * L; bits++) a360508_dbl(one, N, L);
    memcpy(x, one, sizeof(uint64_t) * L);
    /* exponent N-1 (N odd: clear bit 0) */
    for (i = L; i-- > 0;) {
        for (b = 63; b >= 0; b--) {
            uint64_t w = N[i];
            int bit;
            if (i == 0) w &= ~(uint64_t)1;
            bit = (int)((w >> b) & 1);
            a360508_mmul(x, x, x, N, ninv, L, t);
            if (bit) a360508_dbl(x, N, L);
        }
    }
    ok = memcmp(x, one, sizeof(uint64_t) * L) == 0;
    free(one); free(x); free(t);
    return ok;
}

static bool a360508_is_prime(uint64_t m, const uint64_t *N, size_t L)
{
    size_t pi, i;
    (void)m;
    if (L == 1 && N[0] < 4294967296ULL) {
        uint64_t v = N[0], d;
        if (v < 2) return false;
        if (v % 2 == 0) return v == 2;
        for (d = 3; d * d <= v; d += 2)
            if (v % d == 0) return false;
        return true;
    }
    if ((N[0] & 1) == 0) return false;
    for (pi = 0; pi < a360508_npr; pi++) {
        uint64_t p = a360508_pr[pi], r = 0;
        for (i = L; i-- > 0;) {
            r = ((r << 32) | (N[i] >> 32)) % p;
            r = ((r << 32) | (N[i] & 0xffffffffULL)) % p;
        }
        if (r == 0) return false;
    }
    return a360508_fermat2(N, L);
}

static bool a360508_test(uint64_t k)
{
    size_t total = 0, pos = 0, cap, L = 0, i, j;
    uint64_t q;
    char *dig;
    uint64_t *N;
    size_t first;
    bool res;
    for (q = k; q >= 1; q--) {
        unsigned bl = 0;
        uint64_t v = q;
        while (v) { bl++; v >>= 1; }
        total += bl;
    }
    dig = (char *)malloc(total + 1);
    for (q = k; q >= 1; q--) {
        unsigned bl = 0, b;
        uint64_t v = q;
        while (v) { bl++; v >>= 1; }
        for (b = bl; b > 0; b--) dig[pos++] = (char)((q >> (b - 1)) & 1);
    }
    cap = total / 19 + total / 20 + 4;
    N = (uint64_t *)calloc(cap, sizeof(uint64_t));
    first = total % 19;
    if (first == 0) first = 19;
    pos = 0;
    while (pos < total) {
        size_t len = (pos == 0) ? first : 19;
        uint64_t chunk = 0, mul = 1, carry;
        for (j = 0; j < len; j++) {
            chunk = chunk * 10 + (uint64_t)dig[pos + j];
            mul *= 10;
        }
        pos += len;
        carry = chunk;
        for (i = 0; i < L; i++) {
            a360508_u128 s = (a360508_u128)N[i] * mul + carry;
            N[i] = (uint64_t)s;
            carry = (uint64_t)(s >> 64);
        }
        if (carry) N[L++] = carry;
    }
    if (L == 0) L = 1;
    res = a360508_is_prime(k, N, L);
    free(dig);
    free(N);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    a360508_init_primes();
    while (a360508_cnt < (size_t)n) {
        a360508_k++;
        if (a360508_test(a360508_k)) {
            if (a360508_cnt == a360508_cap) {
                a360508_cap = a360508_cap ? a360508_cap * 2 : 32;
                a360508_terms = (uint64_t *)realloc(a360508_terms, a360508_cap * sizeof(uint64_t));
            }
            a360508_terms[a360508_cnt++] = a360508_k;
        }
    }
    return (seq_t)a360508_terms[n - 1];
}
