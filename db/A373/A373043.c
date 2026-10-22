/*
 * A373043  a(n) is the number of products P = j*k*n, 1 < j < k < n, such that
 *          P is a square.
 * https://oeis.org/A373043
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (j,k) : 1 < j < k < n and j*k*n is a perfect square }
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* small primes up to 10000 – used for factorisation */
static int primes[2000];
static size_t prime_cnt = 0;
static bool primes_done = false;

static void init_primes(void)
{
    if (primes_done) return;
    const int MAXP = 10000;
    bool comp[MAXP + 1];
    for (int i = 0; i <= MAXP; ++i) comp[i] = false;
    for (int i = 2; i * i <= MAXP; ++i)
        if (!comp[i])
            for (int j = i * i; j <= MAXP; j += i)
                comp[j] = true;
    for (int i = 2; i <= MAXP; ++i)
        if (!comp[i]) primes[prime_cnt++] = i;
    primes_done = true;
}

/* 128‑bit integer square root – works for all 64‑bit inputs */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967295ULL;   /* sqrt(2^64−1) */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)x) return mid;
        if (sq < (unsigned __int128)x) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

static inline seq_t isqrt_seq(seq_t x)
{
    return (seq_t)isqrt_u64((uint64_t)x);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 4) return 0;

    init_primes();
    uint64_t n64 = (uint64_t)n;

    /* compute squarefree part of n */
    uint64_t tmp = n64;
    seq_t sqf = 1;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        int p = primes[i];
        int odd = 0;
        while (tmp % p == 0) {
            tmp /= p;
            odd ^= 1;
        }
        if (odd) sqf *= (seq_t)p;
    }
    if (tmp > 1) sqf *= (seq_t)tmp;   /* remaining prime factor */

    uint64_t max_prod = (n64 - 1) * (n64 - 2);   /* max j*k with j<k<n */
    seq_t cnt = 0;

    for (uint64_t s = 1; ; ++s) {
        unsigned __int128 prod128 = (unsigned __int128)sqf * s * s;
        if (prod128 > max_prod) break;
        uint64_t m = (uint64_t)prod128;
        uint64_t sqrt_m = (uint64_t)isqrt_u64(m);
        for (uint64_t j = 2; j <= sqrt_m; ++j) {
            if (m % j == 0) {
                uint64_t k = m / j;
                if (j < k && k < n64) ++cnt;
            }
        }
    }
    return cnt;
}
