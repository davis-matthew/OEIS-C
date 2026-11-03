/*
 * A330024  a(n) = floor(n/z) where z is the number of zeros in the decimal
 *          expansion of 2^n, and a(n)=0 when z=0.
 * https://oeis.org/A330024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n / z) where z is the number of zeros in the
 *            decimal expansion of 2^n, and a(n) = 0 when z = 0
 * Range:     u8 n <= 32000, u16 n <= 32000, u32 n <= 32000, u64 n <= 32000,
 *            u128 n <= 32000, big any n
 * Validated: standard, pass, 89 terms through n = 8191 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2^n is kept between calls in base-10^9 limbs (little-endian) that grow as
   needed, doubled up to n (restarting from 1 if a smaller n is asked); the
   zeros are counted limb by limb, all 9 digits of each limb but the top one.
   (The original redid 2^n from scratch in a fixed buffer of 4000 digits,
   which 2^13288 already outgrows.) */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *L = NULL;
static size_t len = 0, cap = 0;
static uint64_t e = 0;                       /* L holds 2^e */

static void reset(void)
{
    if (!cap) {
        cap = 64;
        L = (uint32_t *)malloc(cap * sizeof *L);
        if (!L) abort();
    }
    L[0] = 1;
    len = 1;
    e = 0;
}

static void twice(void)
{
    uint32_t c = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t t = L[j] * 2 + c;           /* < 2 * 10^9 + 1 < 2^32 */
        c = t >= 1000000000u;
        L[j] = c ? t - 1000000000u : t;
    }
    if (c) {
        if (len == cap) {
            uint32_t *t = (uint32_t *)realloc(L, (cap *= 2) * sizeof *L);
            if (!t) abort();
            L = t;
        }
        L[len++] = c;
    }
    ++e;
}

seq_t a(idx_t n)
{
    if (!L || e > n) reset();
    while (e < n) twice();
    uint64_t zeros = 0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t v = L[j];
        int d = 0;
        while (v) { zeros += v % 10 == 0; v /= 10; ++d; }
        if (j + 1 < len) zeros += 9 - d;     /* the limb's leading zeros */
    }
    if (zeros == 0) return (seq_t)0;
    return (seq_t)(n / zeros);
}
