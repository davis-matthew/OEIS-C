/*
 * A377259  Least k such that for every integer i > k there are at least n
 *          composite numbers less than i and relatively prime to i.
 * https://oeis.org/A377259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Least k such that for every integer i > k there are at
 *            least * n composite numbers less than i and relatively prime to
 *            i. * Implementation: compute φ(i), π(i) and the number of
 *            distinct prime factors * of i with a linear sieve up to a fixed
 *            large limit. For each i we * evaluate f(i)=φ(i)-1...
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* choose a generous fixed limit – large enough for all n ≤ 10 000   */
#define LIMIT 2000000

/* global tables */
static uint32_t spf[LIMIT + 1];
static uint64_t phi_arr[LIMIT + 1];
static uint32_t pi_arr[LIMIT + 1];
static uint64_t f_arr[LIMIT + 1];
static uint64_t f_min_next[LIMIT + 2];

static bool initialized = false;

/* linear sieve computing spf and φ */
static void build_sieve(void)
{
    /* primes list – safe size */
    static uint32_t primes[LIMIT / 10 + 10];
    size_t pcnt = 0;

    spf[0] = 0;
    spf[1] = 1;
    phi_arr[0] = 0;
    phi_arr[1] = 1;

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            phi_arr[i] = i - 1;
            primes[pcnt++] = i;
        }

        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > LIMIT) break;
            spf[(uint32_t)ip] = p;
            if (p == spf[i]) {
                phi_arr[(uint32_t)ip] = phi_arr[i] * p;
            } else {
                phi_arr[(uint32_t)ip] = phi_arr[i] * (p - 1);
            }
        }
    }

    /* build prime‑count array π(i) */
    pi_arr[0] = 0;
    pi_arr[1] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        pi_arr[i] = pi_arr[i - 1] + (spf[i] == i ? 1 : 0);
    }
}

/* compute all auxiliary arrays; run once */
static void precompute(void)
{
    if (initialized) return;
    build_sieve();

    /* compute f(i) for i = 2 … LIMIT */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        uint32_t x = i;
        uint32_t cnt = 0;          /* distinct prime factors of i */
        while (x > 1) {
            uint32_t p = spf[x];
            ++cnt;
            while (x % p == 0) x /= p;
        }
        int64_t val = (int64_t)phi_arr[i] - 1LL - (int64_t)pi_arr[i - 1] + (int64_t)cnt;
        f_arr[i] = val > 0 ? (uint64_t)val : 0ULL;
    }

    /* suffix minima of f:  f_min_next[i] = min_{j≥i} f[j]   */
    f_min_next[LIMIT + 1] = UINT64_MAX;  /* sentinel large value */
    for (uint32_t i = LIMIT; i >= 2; --i) {
        f_min_next[i] = f_arr[i] < f_min_next[i + 1] ? f_arr[i] : f_min_next[i + 1];
    }

    initialized = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined in OEIS – guard */
    precompute();

    /* binary search for smallest i with f_min_next[i] ≥ n   */
    idx_t lo = 2;
    idx_t hi = LIMIT + 1;            /* inclusive sentinel */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (f_min_next[mid] >= n) hi = mid;
        else                       lo = mid + 1;
    }
    /* lo is the first i such that min_{j≥i} f[j] ≥ n   */
    /* answer k = i - 1   */
    return (seq_t)(lo - 1);
}
