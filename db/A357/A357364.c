/*
 * A357364  Primes p such that either p^(q-1) == 1 (mod q^2) or q^(p-1) == 1
 *          (mod p^2), where q = A151800(A151800(A151800(p))).
 * https://oeis.org/A357364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search over consecutive primes p with q =
 *            A151800(A151800(A151800(p))); list q whenever p^(q-1) == 1 (mod
 *            q^2) or q^(p-1) == 1 (mod p^2). The listed terms are the larger
 *            prime q of each pair, e.g. (3, 11), (13, 23), (29, 41)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 a357364_u128;

/* ---- prime generator (segmented sieve, sequential) ---- */
#define A357364_SEG (1u << 20)
static uint64_t *a357364_base = 0;
static size_t a357364_nbase = 0, a357364_capbase = 0;
static uint64_t a357364_lo = 0;           /* next segment start */
static uint64_t *a357364_buf = 0;
static size_t a357364_nbuf = 0, a357364_ibuf = 0;

static void a357364_extend_base(uint64_t need)
{
    uint64_t c;
    if (a357364_nbase == 0) {
        a357364_capbase = 1024;
        a357364_base = (uint64_t *)malloc(a357364_capbase * sizeof(uint64_t));
        a357364_base[a357364_nbase++] = 3;
    }
    c = a357364_base[a357364_nbase - 1] + 2;
    while (a357364_base[a357364_nbase - 1] < need) {
        bool pr = true;
        size_t i;
        for (i = 0; i < a357364_nbase; i++) {
            uint64_t b = a357364_base[i];
            if (b * b > c) break;
            if (c % b == 0) { pr = false; break; }
        }
        if (pr) {
            if (a357364_nbase == a357364_capbase) {
                a357364_capbase *= 2;
                a357364_base = (uint64_t *)realloc(a357364_base, a357364_capbase * sizeof(uint64_t));
            }
            a357364_base[a357364_nbase++] = c;
        }
        c += 2;
    }
}

static void a357364_fill(void)
{
    uint64_t lo = a357364_lo, hi = lo + A357364_SEG, s;
    unsigned char *flag;
    size_t i;
    if (!a357364_buf) a357364_buf = (uint64_t *)malloc(A357364_SEG * sizeof(uint64_t));
    flag = (unsigned char *)malloc(A357364_SEG);
    for (i = 0; i < A357364_SEG; i++) flag[i] = 1;
    s = 1;
    while (s * s < hi) s++;
    a357364_extend_base(s + 1);
    for (i = 0; i < a357364_nbase; i++) {
        uint64_t b = a357364_base[i], st;
        if (b * b >= hi) break;
        st = (lo + b - 1) / b * b;
        if (st < b * b) st = b * b;
        for (; st < hi; st += b) flag[st - lo] = 0;
    }
    a357364_nbuf = 0;
    a357364_ibuf = 0;
    for (i = 0; i < A357364_SEG; i++) {
        uint64_t v = lo + i;
        if (v < 2) continue;
        if (v == 2 || ((v & 1) && flag[i])) a357364_buf[a357364_nbuf++] = v;
    }
    free(flag);
    a357364_lo = hi;
}

static uint64_t a357364_next(void)
{
    while (a357364_ibuf >= a357364_nbuf) a357364_fill();
    return a357364_buf[a357364_ibuf++];
}

/* ---- modular power ---- */
static uint64_t a357364_redc(a357364_u128 T, uint64_t n, uint64_t ninv)
{
    uint64_t m = (uint64_t)T * ninv;
    uint64_t t = (uint64_t)((T + (a357364_u128)m * n) >> 64);
    return t >= n ? t - n : t;
}

/* returns true iff base^e == 1 (mod n) */
static bool a357364_pow_is_one(uint64_t base, uint64_t e, uint64_t n)
{
    if ((n & 1) && n < ((uint64_t)1 << 63)) {
        uint64_t x = n, ninv, one, r2, b, r;
        int i;
        for (i = 0; i < 6; i++) x *= 2 - n * x;
        ninv = (uint64_t)0 - x;
        one = ((uint64_t)0 - n) % n;
        r2 = (uint64_t)(((a357364_u128)one * one) % n);
        b = a357364_redc((a357364_u128)(base % n) * r2, n, ninv);
        r = one;
        while (e) {
            if (e & 1) r = a357364_redc((a357364_u128)r * b, n, ninv);
            b = a357364_redc((a357364_u128)b * b, n, ninv);
            e >>= 1;
        }
        return r == one;
    } else {
        a357364_u128 r = 1 % n, b = base % n;
        while (e) {
            if (e & 1) r = (r * b) % n;
            b = (b * b) % n;
            e >>= 1;
        }
        return r == 1 % n;
    }
}

static uint64_t *a357364_terms = 0;
static size_t a357364_nterms = 0, a357364_capterms = 0;
static uint64_t a357364_w[4];
static bool a357364_init = false;

seq_t a(idx_t n)
{
    if (!a357364_init) {
        int k;
        a357364_lo = 0;
        for (k = 0; k < 4; k++) a357364_w[k] = a357364_next();   /* 2,3,5,7 */
        a357364_init = true;
    }
    while (a357364_nterms < (size_t)n) {
        uint64_t p = a357364_w[0], q = a357364_w[3];
        bool ok;
        ok = a357364_pow_is_one(p, q - 1, q * q) ||
             a357364_pow_is_one(q, p - 1, p * p);
        if (ok) {
            if (a357364_nterms == a357364_capterms) {
                a357364_capterms = a357364_capterms ? a357364_capterms * 2 : 64;
                a357364_terms = (uint64_t *)realloc(a357364_terms, a357364_capterms * sizeof(uint64_t));
            }
            a357364_terms[a357364_nterms++] = q;
        }
        a357364_w[0] = a357364_w[1];
        a357364_w[1] = a357364_w[2];
        a357364_w[2] = a357364_w[3];
        a357364_w[3] = a357364_next();
    }
    return (seq_t)a357364_terms[(size_t)n - 1];
}
