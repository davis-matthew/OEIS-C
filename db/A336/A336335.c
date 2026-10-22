/*
 * A336335  a(n) is the index of the first occurrence of the Euclidean
 *          distance prime(n) from a point on a square spiral to its starting
 *          point at 1.
 * https://oeis.org/A336335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the first lattice point whose Euclidean *
 *            distance from the origin equals the n-th prime on the Ulam
 *            square * spiral
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   coord_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Required type aliases ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/coord_index.h"

/* ---------- Helper: integer square root (uint64_t) ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n, y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---------- Helper: primality test for uint64_t ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ---------- Prime storage and generator ---------- */
static uint64_t *prime_table = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t last_candidate = 1;

static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;

    if (prime_table == NULL) {
        prime_cap = 1024;
        prime_table = (uint64_t *)malloc(sizeof(uint64_t) * prime_cap);
        last_candidate = 2;
    }

    while (prime_cnt < (size_t)n) {
        uint64_t p = last_candidate;
        while (!is_prime_uint64(p))
            ++p;
        /* store prime */
        if (prime_cnt == prime_cap) {
            prime_cap *= 2;
            prime_table = (uint64_t *)realloc(prime_table, sizeof(uint64_t) * prime_cap);
        }
        prime_table[prime_cnt++] = p;
        last_candidate = p + 1;
    }
}

/* ---------- Find a, b such that a^2 + b^2 = p (p is prime ≡ 1 mod 4) ---------- */
static bool find_ab(uint64_t p, uint64_t *a_out, uint64_t *b_out)
{
    if (p == 2) return false;

    for (uint64_t a = 1; a * a <= p; ++a) {
        uint64_t b2 = p - a * a;
        uint64_t b_s = isqrt_u64(b2);
        if (b_s * b_s == b2) {
            *a_out = a;
            *b_out = b_s;
            return true;
        }
    }
    return false;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1;   /* offset is 1 */

    ensure_primes(n);
    if (n > prime_cnt) return (seq_t)-1;   /* should not happen */

    uint64_t prime = prime_table[n - 1];

    /* Candidate points: axis points (±p,0),(0,±p) */
    seq_t best = (seq_t)-1;
    long long p_ll = (long long)prime;

    long long ax[4] = { p_ll, -p_ll, 0, 0 };
    long long ay[4] = { 0, 0, p_ll, -p_ll };

    for (int i = 0; i < 4; ++i) {
        seq_t idx = coord_index(ax[i], ay[i]);
        if (best == (seq_t)-1 || idx < best) best = idx;
    }

    /* Additional points if prime ≡ 1 (mod 4) */
    if ((prime & 3) == 1) {
        uint64_t a, b;
        if (find_ab(prime, &a, &b)) {
            /* coordinates from Gaussian representation */
            long long x1 = (long long)((uint64_t)a * a - (uint64_t)b * b);
            long long y1 = (long long)((uint64_t)2 * a * b);

            long long xs[4] = {  x1, -x1,  y1, -y1 };
            long long ys[4] = {  y1, -y1,  x1, -x1 };

            for (int i = 0; i < 4; ++i) {
                seq_t idx = coord_index(xs[i], ys[i]);
                if (best == (seq_t)-1 || idx < best) best = idx;
            }
        }
    }

    return best;
}
