/*
 * A366196  The number of ways to express n^n in the form a^b for positive
 *          integers a and b.
 * https://oeis.org/A366196
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n * g), where g = gcd of the exponents in the prime
 *            factorisation of n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Small integer gcd */
static int gcd_int(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Factorise unsigned 64‑bit integer n.
   pf   : array where prime factors are stored
   pn   : corresponding exponents
   cnt  : number of distinct prime factors
   The arrays must be large enough (e.g. 64 elements). */
static void factor_uint64(uint64_t n, uint64_t pf[64], int pn[64], size_t *cnt) {
    size_t idx = 0;
    if (n == 0) { *cnt = 0; return; }
    while ((n & 1ULL) == 0ULL) {
        if (idx == 0 || pf[idx - 1] != 2ULL) {
            pf[idx] = 2ULL;
            pn[idx] = 0;
            ++idx;
        }
        ++pn[idx - 1];
        n >>= 1;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0ULL) {
            pf[idx] = d;
            pn[idx] = 0;
            ++idx;
            while (n % d == 0ULL) {
                ++pn[idx - 1];
                n /= d;
            }
        }
    }
    if (n > 1ULL) {
        pf[idx] = n;
        pn[idx] = 1;
        ++idx;
    }
    *cnt = idx;
}

/* Integer square root (floor) for unsigned 64‑bit numbers */
static idx_t _floor_isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) {
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* offset behaviour not defined below 2 */

    /* factorise n */
    uint64_t p_n[64];
    int    e_n[64];
    size_t cnt_n;
    factor_uint64((uint64_t)n, p_n, e_n, &cnt_n);

    /* compute g = gcd of the exponents */
    int g = e_n[0];
    for (size_t i = 1; i < cnt_n; ++i) {
        g = gcd_int(g, e_n[i]);
    }

    /* factorise g */
    uint64_t p_g[64];
    int    e_g[64];
    size_t cnt_g;
    factor_uint64((uint64_t)g, p_g, e_g, &cnt_g);

    /* combine primes from n and g to count divisors of N = n * g */
    seq_t divcnt = 1;

    /* contributions from primes that appear in n */
    for (size_t i = 0; i < cnt_n; ++i) {
        int add = 0;
        for (size_t j = 0; j < cnt_g; ++j) {
            if (p_g[j] == p_n[i]) {
                add = e_g[j];
                break;
            }
        }
        divcnt *= (seq_t)(e_n[i] + add + 1);
    }

    /* contributions from primes that appear only in g */
    for (size_t j = 0; j < cnt_g; ++j) {
        bool found = false;
        for (size_t i = 0; i < cnt_n; ++i) {
            if (p_g[j] == p_n[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            divcnt *= (seq_t)(e_g[j] + 1);
        }
    }

    return divcnt;
}
