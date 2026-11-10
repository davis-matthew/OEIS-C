/*
 * A384064  a(n) = s(n) divided by the smallest multiple prime factor of s(n),
 *          where s = A013929.
 * https://oeis.org/A384064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A013929(n)/A046027(n); A013929 found by binary search
 *            with Moebius counting, then the smallest prime p with p^2 | s is
 *            found by trial division and a(n) = s/p
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9187 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline idx_t A013929_floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 p = (unsigned __int128)mid * mid;
        if (p <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static int8_t *A013929_mu_vals = NULL;
static size_t A013929_mu_size = 0;

static void A013929_recompute_mu(size_t n)
{
    if (n <= A013929_mu_size) return;
    A013929_mu_vals = (int8_t *)realloc(A013929_mu_vals, (n + 1) * sizeof(int8_t));
    unsigned char *comp = (unsigned char *)calloc(n + 1, 1);
    uint32_t *primes = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    size_t pc = 0;
    A013929_mu_vals[0] = 0;
    A013929_mu_vals[1] = 1;
    for (size_t i = 2; i <= n; ++i) {
        if (!comp[i]) {
            primes[pc++] = (uint32_t)i;
            A013929_mu_vals[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint64_t prod = (uint64_t)i * primes[j];
            if (prod > n) break;
            comp[prod] = 1;
            if (i % primes[j] == 0) {
                A013929_mu_vals[prod] = 0;
                break;
            } else {
                A013929_mu_vals[prod] = (int8_t)(-A013929_mu_vals[i]);
            }
        }
    }
    free(comp);
    free(primes);
    A013929_mu_size = n;
}

static idx_t A013929_count(idx_t x)
{
    idx_t lim = A013929_floor_sqrt(x);
    if (lim > A013929_mu_size) A013929_recompute_mu((size_t)lim);
    idx_t sp = 0, sn = 0;
    for (idx_t i = 1; i <= lim; ++i) {
        int8_t mu = A013929_mu_vals[i];
        if (mu == 0) continue;
        idx_t term = x / (i * i);
        if (mu > 0) sp += term; else sn += term;
    }
    return x - sp + sn;
}

static idx_t A013929_idx(idx_t n)
{
    idx_t high = n * 3 + 10;
    while (A013929_count(high) < n)
        high <<= 1;
    idx_t low = 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (A013929_count(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t s = A013929_idx(n);
    for (idx_t p = 2; p * p <= s; ++p) {
        if (s % (p * p) == 0)
            return (seq_t)(s / p);
    }
    return (seq_t)s;
}
