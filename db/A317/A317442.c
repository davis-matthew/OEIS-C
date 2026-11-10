/*
 * A317442  a(1) = a(2) = 1; for n >= 3, a(n) = a(t(n)) + a(n-t(n)) where t =
 *          A000195.
 * https://oeis.org/A317442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(t(n)) + a(n - t(n)), t(n) = A000195(n) = floor(log(n));
 *            a(1) = a(2) = 1
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

#define A195_FL 8
#define A195_IL 3
#define A195_L (A195_FL + A195_IL)
#define A195_KMAX 44

static uint64_t a195_thr[A195_KMAX + 1];
static int a195_ready = 0;

static void a195_mul(uint32_t *r, const uint32_t *x, const uint32_t *y)
{
    uint32_t t[2 * A195_L];
    int i, j;
    memset(t, 0, sizeof(t));
    for (i = 0; i < A195_L; i++) {
        uint64_t carry = 0;
        for (j = 0; j < A195_L; j++) {
            uint64_t cur = (uint64_t)x[i] * y[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        t[i + A195_L] = (uint32_t)carry;
    }
    for (i = 0; i < A195_L; i++)
        r[i] = t[i + A195_FL];
}

static void a195_add(uint32_t *r, const uint32_t *x)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A195_L; i++) {
        uint64_t cur = (uint64_t)r[i] + x[i] + carry;
        r[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a195_div_small(uint32_t *r, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = A195_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | r[i];
        r[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a195_init(void)
{
    uint32_t e[A195_L], term[A195_L], p[A195_L];
    uint32_t j;
    int k;
    memset(e, 0, sizeof(e));
    memset(term, 0, sizeof(term));
    term[A195_FL] = 1;
    for (j = 1; j <= 80; j++) {
        a195_add(e, term);
        a195_div_small(term, j);
    }
    memset(p, 0, sizeof(p));
    p[A195_FL] = 1;
    for (k = 1; k <= A195_KMAX; k++) {
        a195_mul(p, p, e);
        a195_thr[k] = ((uint64_t)p[A195_FL + 1] << 32) | p[A195_FL];
    }
    a195_ready = 1;
}

static uint64_t A000195_t(uint64_t n)
{
    int k;
    uint64_t c = 0;
    if (!a195_ready)
        a195_init();
    for (k = 1; k <= A195_KMAX; k++) {
        if (n > a195_thr[k])
            c++;
        else
            break;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t *v;
    seq_t res;
    uint64_t i, t;
    if (n <= 2)
        return (seq_t)1;
    v = mem_new(seq_t, (size_t)n + 1);
    v[1] = (seq_t)1;
    v[2] = (seq_t)1;
    for (i = 3; i <= (uint64_t)n; i++) {
        t = A000195_t(i);
        v[i] = v[t] + v[i - t];
    }
    res = v[n];
    mem_free(v);
    return res;
}
