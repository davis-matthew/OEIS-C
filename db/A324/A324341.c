/*
 * A324341  If 2n = 2^e1 + ... + 2^ek [e1 .. ek distinct], then a(n) is the
 *          number of nonzero digits when A002110(e1) * ... * A002110(ek) is
 *          written in primorial base.
 * https://oeis.org/A324341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A267263(A283477(n)): number of nonzero digits in the
 *            primorial-base representation of prod_{bit k of n set}
 *            A002110(k+1). Computed with a private bignum
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 9891 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A324341_MAXL 2048

static uint32_t a324341_nextprime(uint32_t p)
{
    uint32_t c = p + 1;
    for (;; c++) {
        int ok = 1;
        uint32_t d;
        if (c < 2) continue;
        for (d = 2; (uint64_t)d * d <= c; d++) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) return c;
    }
}

static void a324341_mulsmall(uint32_t *x, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
}

static uint32_t a324341_divsmall(uint32_t *x, size_t *len, uint32_t d)
{
    uint64_t rem = 0;
    size_t i = *len;
    while (i > 0) {
        i--;
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (*len > 0 && x[*len - 1] == 0) (*len)--;
    return (uint32_t)rem;
}

seq_t a(idx_t n)
{
    static uint32_t big[A324341_MAXL];
    size_t len = 1;
    int k, j;
    uint32_t p;
    seq_t count = 0;

    big[0] = 1;
    for (k = 0; k < 64; k++) {
        if ((n >> k) & 1) {
            p = 1;
            for (j = 0; j <= k; j++) {
                p = a324341_nextprime(p);
                a324341_mulsmall(big, &len, p);
            }
        }
    }

    p = 1;
    while (len > 0) {
        uint32_t r;
        p = a324341_nextprime(p);
        r = a324341_divsmall(big, &len, p);
        if (r != 0) count++;
    }
    return count;
}
