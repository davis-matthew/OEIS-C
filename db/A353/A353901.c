/*
 * A353901  Numbers k such that A353900(k) is divisible by k.
 * https://oeis.org/A353901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A353900(k) == 0 (mod k), where A353900
 *            is multiplicative with A353900(p^e) = 1 + sum_{j>=0, 2^j<=e}
 *            p^(2^j); computed by a segmented sieve search
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A353901_SEG (1u << 18)
#define A353901_MAXT 128
#define A353901_PLIM (1u << 21)

static uint64_t A353901_terms[A353901_MAXT];
static int A353901_cnt = 0;
static uint64_t A353901_next = 1;   /* next unsearched k */
static uint32_t *A353901_pr = 0;
static size_t A353901_np = 0;

static void A353901_init(void)
{
    if (A353901_pr) return;
    unsigned char *c = (unsigned char *)calloc(A353901_PLIM + 1, 1);
    A353901_pr = (uint32_t *)malloc(sizeof(uint32_t) * 160000);
    for (uint32_t i = 2; i <= A353901_PLIM; i++) {
        if (!c[i]) {
            if (A353901_np < 160000) A353901_pr[A353901_np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A353901_PLIM; j += i) c[j] = 1;
        }
    }
    free(c);
}

static void A353901_segment(void)
{
    static uint64_t rem[A353901_SEG];
    static uint64_t prod[A353901_SEG];
    uint64_t L = A353901_next, H = L + A353901_SEG;
    for (uint32_t i = 0; i < A353901_SEG; i++) { rem[i] = L + i; prod[i] = 1; }
    for (size_t pi = 0; pi < A353901_np; pi++) {
        uint64_t p = A353901_pr[pi];
        if (p * p >= H) break;
        uint64_t m = ((L + p - 1) / p) * p;
        for (; m < H; m += p) {
            uint32_t i = (uint32_t)(m - L);
            uint64_t r = rem[i];
            unsigned e = 0;
            while (r % p == 0) { r /= p; e++; }
            rem[i] = r;
            uint64_t s = 1, pw = p;
            unsigned cur = 1;
            while (cur <= e) {
                s += pw;
                if (cur > e / 2) break;
                pw *= pw;
                cur <<= 1;
            }
            prod[i] *= s;
        }
    }
    for (uint32_t i = 0; i < A353901_SEG; i++) {
        uint64_t f = prod[i];
        if (rem[i] > 1) f *= (1 + rem[i]);
        uint64_t k = L + i;
        if (f % k == 0 && A353901_cnt < A353901_MAXT)
            A353901_terms[A353901_cnt++] = k;
    }
    A353901_next = H;
}

seq_t a(idx_t n)
{
    A353901_init();
    while ((idx_t)A353901_cnt < n) A353901_segment();
    return (seq_t)A353901_terms[n - 1];
}
