/*
 * A372052  Where n appears in A370952, or -1 if n never appears there.
 * https://oeis.org/A372052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- generate A370952 (a(1)=1; a(k) = least unused positive
 *            integer with a(k) == a(k-1) (mod 2k-1)) and report the index k
 *            where a(k) = n; -1 if n does not occur within the generated
 *            range
 * Range:     u8 n <= 25, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8639 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A372052_CAP    12000000ULL          /* max index of A370952 generated */
#define A372052_BITLIM (1ULL << 28)         /* values below: bitset          */
#define A372052_POSLIM (1ULL << 21)         /* values below: position stored */

static uint8_t  *A372052_bits = NULL;
static uint64_t *A372052_htab = NULL;
static size_t    A372052_hcap = 0, A372052_hcnt = 0;
static uint32_t *A372052_pos = NULL;
static uint64_t  A372052_prev = 0, A372052_idx = 0;

static size_t A372052_hslot(uint64_t x, size_t cap)
{
    return (size_t)((x * 11400714819323198485ULL) >> 20) & (cap - 1);
}

static bool A372052_hhas(uint64_t x)
{
    if (A372052_hcap == 0) return false;
    size_t h = A372052_hslot(x, A372052_hcap);
    while (A372052_htab[h] != 0) {
        if (A372052_htab[h] == x) return true;
        h = (h + 1) & (A372052_hcap - 1);
    }
    return false;
}

static void A372052_hins_raw(uint64_t *tab, size_t cap, uint64_t x)
{
    size_t h = A372052_hslot(x, cap);
    while (tab[h] != 0) h = (h + 1) & (cap - 1);
    tab[h] = x;
}

static void A372052_hadd(uint64_t x)
{
    if (2 * (A372052_hcnt + 1) > A372052_hcap) {
        size_t ncap = A372052_hcap ? A372052_hcap * 2 : 65536;
        uint64_t *nt = (uint64_t *)calloc(ncap, sizeof(uint64_t));
        for (size_t i = 0; i < A372052_hcap; ++i)
            if (A372052_htab[i]) A372052_hins_raw(nt, ncap, A372052_htab[i]);
        free(A372052_htab);
        A372052_htab = nt;
        A372052_hcap = ncap;
    }
    A372052_hins_raw(A372052_htab, A372052_hcap, x);
    A372052_hcnt++;
}

static bool A372052_used(uint64_t x)
{
    if (x < A372052_BITLIM) return (A372052_bits[x >> 3] >> (x & 7)) & 1;
    return A372052_hhas(x);
}

static void A372052_mark(uint64_t x, uint64_t k)
{
    if (x < A372052_BITLIM) A372052_bits[x >> 3] |= (uint8_t)(1u << (x & 7));
    else A372052_hadd(x);
    if (x < A372052_POSLIM) A372052_pos[x] = (uint32_t)k;
}

seq_t a(idx_t n)
{
    if (A372052_bits == NULL) {
        A372052_bits = (uint8_t *)calloc((size_t)(A372052_BITLIM / 8), 1);
        A372052_pos = (uint32_t *)calloc((size_t)A372052_POSLIM, sizeof(uint32_t));
        A372052_idx = 1;
        A372052_prev = 1;
        A372052_mark(1, 1);
    }
    if ((uint64_t)n >= A372052_POSLIM) return (seq_t)-1;
    uint64_t t = (uint64_t)n;

    while (A372052_pos[t] == 0 && A372052_idx < A372052_CAP) {
        uint64_t k = A372052_idx + 1;
        uint64_t m = 2 * k - 1;
        uint64_t c = A372052_prev % m;
        if (c == 0) c = m;
        while (A372052_used(c)) c += m;
        A372052_mark(c, k);
        A372052_prev = c;
        A372052_idx = k;
    }

    if (A372052_pos[t] == 0) return (seq_t)-1;
    return (seq_t)A372052_pos[t];
}
