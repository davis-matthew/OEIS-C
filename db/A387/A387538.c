/*
 * A387538  Indices of record high points in A387537.
 * https://oeis.org/A387538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th index m at which A387537(m) strictly exceeds all
 *            earlier values; found by a segmented omega sieve scan, where
 *            A387537(m) is the length of the longest run omega(m) <
 *            omega(m+1) <
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A387538_B 262144
#define A387538_OV 40

static uint64_t A387538_rec[64];
static unsigned A387538_nrec = 0;
static unsigned A387538_best = 0;
static uint64_t A387538_next = 1;
static uint32_t *A387538_pr = NULL;
static size_t A387538_np = 0;
static uint64_t A387538_plim = 0;

static void A387538_primes(uint64_t lim)
{
    if (lim <= A387538_plim) return;
    uint64_t nl = lim * 2 < 1000 ? 1000 : lim * 2;
    unsigned char *s = (unsigned char *)calloc((size_t)nl + 1, 1);
    uint32_t *p = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)nl / 2 + 10));
    if (!s || !p) abort();
    size_t c = 0;
    for (uint64_t i = 2; i <= nl; ++i) {
        if (!s[i]) {
            p[c++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= nl; j += i) s[j] = 1;
        }
    }
    free(s);
    free(A387538_pr);
    A387538_pr = p;
    A387538_np = c;
    A387538_plim = nl;
}

static void A387538_scan(void)
{
    static uint64_t rem[A387538_B + A387538_OV];
    static unsigned char om[A387538_B + A387538_OV];
    static unsigned char L[A387538_B + A387538_OV];
    uint64_t lo = A387538_next;
    size_t len = A387538_B + A387538_OV;
    uint64_t hi = lo + len - 1;
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= hi) ++r;
    A387538_primes(r + 1);
    for (size_t i = 0; i < len; ++i) { rem[i] = lo + i; om[i] = 0; }
    for (size_t k = 0; k < A387538_np; ++k) {
        uint64_t p = A387538_pr[k];
        if (p * p > hi) break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t m = st; m <= hi; m += p) {
            size_t i = (size_t)(m - lo);
            ++om[i];
            do { rem[i] /= p; } while (rem[i] % p == 0);
        }
    }
    for (size_t i = 0; i < len; ++i) if (rem[i] > 1) ++om[i];
    L[len - 1] = 1;
    for (size_t i = len - 1; i-- > 0;) {
        L[i] = (om[i] < om[i + 1]) ? (unsigned char)(L[i + 1] + 1) : 1;
    }
    for (size_t i = 0; i < A387538_B; ++i) {
        if (L[i] > A387538_best) {
            A387538_best = L[i];
            if (A387538_nrec < 64) A387538_rec[A387538_nrec++] = lo + i;
        }
    }
    A387538_next = lo + A387538_B;
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 64) return (seq_t)0;
    while (A387538_nrec < n) A387538_scan();
    return (seq_t)A387538_rec[n - 1];
}
