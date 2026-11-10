/*
 * A396788  (10^n)-th brilliant number.
 * https://oeis.org/A396788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = A078972(10^n); computed by counting brilliant numbers <=
 *            x with a prime sieve and pi(x/p) lookups, bracketing x, then
 *            enumerating the small final window
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t bq_p10[20];
static int bq_built_D = 0;
static uint64_t *bq_comp = NULL;
static uint32_t *bq_cnt = NULL;
static uint32_t *bq_primes = NULL;
static uint32_t bq_total = 0;
static uint32_t bq_sidx[12], bq_eidx[12];

static unsigned bq_popcnt(uint64_t x)
{
#ifdef __GNUC__
    return (unsigned)__builtin_popcountll(x);
#else
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (unsigned)((x * 0x0101010101010101ULL) >> 56);
#endif
}

static uint64_t bq_pi(uint64_t y)
{
    if (y < 2) return 0;
    uint64_t idx = (y - 1) >> 1;
    uint64_t w = idx >> 6;
    unsigned b = (unsigned)(idx & 63);
    uint64_t mask = (b == 63) ? ~0ULL : ((1ULL << (b + 1)) - 1ULL);
    return 1ULL + bq_cnt[w] + bq_popcnt(~bq_comp[w] & mask);
}

static void bq_ensure(int D)
{
    if (D <= bq_built_D) return;
    if (D > 9) D = 9;
    if (D <= bq_built_D) return;
    free(bq_comp); free(bq_cnt); free(bq_primes);
    uint64_t L = bq_p10[D] - 1;
    uint64_t nbits = (L + 1) / 2;
    uint64_t words = (nbits + 63) / 64;
    bq_comp = (uint64_t *)calloc((size_t)words + 1, sizeof(uint64_t));
    bq_cnt = (uint32_t *)calloc((size_t)words + 1, sizeof(uint32_t));
    if (!bq_comp || !bq_cnt) abort();
    bq_comp[0] |= 1ULL;
    for (uint64_t p = 3; p * p <= L; p += 2) {
        uint64_t i = p >> 1;
        if (!((bq_comp[i >> 6] >> (i & 63)) & 1ULL)) {
            for (uint64_t m = p * p; m <= L; m += 2 * p) {
                uint64_t j = m >> 1;
                bq_comp[j >> 6] |= 1ULL << (j & 63);
            }
        }
    }
    uint32_t run = 0;
    for (uint64_t w = 0; w < words; ++w) {
        bq_cnt[w] = run;
        uint64_t word = ~bq_comp[w];
        if (w == words - 1 && (nbits & 63)) word &= (1ULL << (nbits & 63)) - 1ULL;
        run += bq_popcnt(word);
    }
    bq_total = run + 1;
    bq_primes = (uint32_t *)malloc((size_t)bq_total * sizeof(uint32_t));
    if (!bq_primes) abort();
    size_t k = 0;
    bq_primes[k++] = 2;
    for (uint64_t i = 0; i < nbits; ++i)
        if (!((bq_comp[i >> 6] >> (i & 63)) & 1ULL))
            bq_primes[k++] = (uint32_t)(2 * i + 1);
    bq_built_D = D;
    for (int d = 1; d <= D; ++d) {
        bq_sidx[d] = (uint32_t)bq_pi(bq_p10[d - 1] - 1);
        bq_eidx[d] = (uint32_t)bq_pi(bq_p10[d] - 1);
    }
}

static int bq_digits(uint64_t x)
{
    int d = 0;
    while (x) { ++d; x /= 10; }
    return d;
}

static int bq_D(uint64_t x)
{
    int D = (bq_digits(x) + 1) / 2;
    if (D < 1) D = 1;
    return D;
}

/* number of brilliant numbers <= x */
static uint64_t bq_count(uint64_t x)
{
    if (x < 4) return 0;
    int D = bq_D(x);
    bq_ensure(D);
    uint64_t total = 0;
    for (int d = 1; d <= D; ++d) {
        if (bq_p10[d - 1] * bq_p10[d - 1] > x) break;
        uint64_t hid = bq_p10[d] - 1;
        uint32_t e = bq_eidx[d];
        for (uint32_t i = bq_sidx[d]; i < e; ++i) {
            uint64_t p = bq_primes[i];
            if (p * p > x) break;
            uint64_t m = x / p;
            uint64_t top = (m >= hid) ? e : bq_pi(m);
            total += top - i;
        }
    }
    return total;
}

static int bq_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    static uint64_t memo[20];
    static bool have[20];
    if (n > 19) return (seq_t)0;
    if (have[n]) return (seq_t)memo[n];
    bq_p10[0] = 1;
    for (int i = 1; i < 20; ++i) bq_p10[i] = bq_p10[i - 1] * 10ULL;
    uint64_t target = bq_p10[n];

    uint64_t lo = 0, hi = 4, clo = 0, chi;
    chi = bq_count(hi);
    while (chi < target) {
        lo = hi; clo = chi;
        hi *= 2;
        chi = bq_count(hi);
    }
    int it = 0;
    while (chi - clo > 1000000ULL && hi - lo > 1) {
        uint64_t mid;
        if (it & 1) {
            mid = lo + (hi - lo) / 2;
        } else {
            uint64_t step = (hi - lo) / (chi - clo);
            if (step < 1) step = 1;
            mid = lo + (target - clo) * step;
        }
        if (mid <= lo) mid = lo + 1;
        if (mid >= hi) mid = hi - 1;
        ++it;
        uint64_t cm = bq_count(mid);
        if (cm >= target) { hi = mid; chi = cm; }
        else { lo = mid; clo = cm; }
    }

    /* enumerate brilliant numbers in (lo, hi] */
    size_t cap = (size_t)(chi - clo) + 4;
    uint64_t *arr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!arr) abort();
    size_t cnt = 0;
    int D = bq_D(hi);
    bq_ensure(D);
    for (int d = 1; d <= D; ++d) {
        if (bq_p10[d - 1] * bq_p10[d - 1] > hi) break;
        uint64_t hid = bq_p10[d] - 1;
        for (uint32_t i = bq_sidx[d]; i < bq_eidx[d]; ++i) {
            uint64_t p = bq_primes[i];
            if (p * p > hi) break;
            uint64_t qmin = lo / p + 1;
            if (qmin < p) qmin = p;
            uint64_t qmax = hi / p;
            if (qmax > hid) qmax = hid;
            if (qmin > qmax) continue;
            uint64_t i0 = bq_pi(qmin - 1), i1 = bq_pi(qmax);
            for (uint64_t j = i0; j < i1; ++j) {
                if (cnt >= cap) { cap *= 2; arr = (uint64_t *)realloc(arr, cap * sizeof(uint64_t)); if (!arr) abort(); }
                arr[cnt++] = p * (uint64_t)bq_primes[j];
            }
        }
    }
    qsort(arr, cnt, sizeof(uint64_t), bq_cmp);
    uint64_t res = arr[(size_t)(target - clo - 1)];
    free(arr);
    memo[n] = res; have[n] = true;
    return (seq_t)res;
}
