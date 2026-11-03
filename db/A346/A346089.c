/*
 * A346089  a(n) = n divided by the smallest divisor d of n for which
 *          A002034(d) = A002034(n), where A002034(n) is the smallest positive
 *          integer k such that k! is a multiple of n.
 * https://oeis.org/A346089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / d, where d is the smallest divisor of n such that the
 *            Smarandache function S(d) equals S(n). S(m) is the minimal
 *            integer k with k! divisible by m
 * Range:     u8 n <= 1372, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9076 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t smarandache(idx_t n) {
    if (n <= 1) return 1;
    idx_t temp = n;
    seq_t best = 0;                 /* highest k needed among prime powers */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* binary search for minimal k with sum_{i} floor(k/p^i) >= e */
            idx_t lo = 1, hi = p * e;           /* hi is a safe upper bound */
            while (lo < hi) {
                idx_t mid = (lo + hi) >> 1;
                idx_t sum = 0, d = mid;
                while (d) {
                    d /= p;
                    sum += d;
                }
                if (sum >= e)
                    hi = mid;
                else
                    lo = mid + 1;
            }
            if (lo > best) best = lo;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        idx_t p = temp, e = 1;
        idx_t lo = 1, hi = p * e;
        while (lo < hi) {
            idx_t mid = (lo + hi) >> 1;
            idx_t sum = 0, d = mid;
            while (d) {
                d /= p;
                sum += d;
            }
            if (sum >= e)
                hi = mid;
            else
                lo = mid + 1;
        }
        if (lo > best) best = lo;
    }
    return best;
}

/* Recursively examine all divisors of n, keeping the smallest d
   such that smarandache(d) == target.  cur is the current divisor. */
static void dfs_divisors(idx_t idx, idx_t cur,
                         const idx_t primes[], const idx_t exps[],
                         idx_t len, seq_t *best, seq_t target) {
    if (idx == len) {
        if (cur > 0) {
            seq_t s = smarandache(cur);
            if (s == target) {
                if (*best == 0 || cur < *best)
                    *best = cur;
            }
        }
        return;
    }
    idx_t val = 1;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        dfs_divisors(idx + 1, cur * val, primes, exps, len, best, target);
        val *= primes[idx];
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* a(0) undefined, but harmless */
    seq_t s_n = smarandache(n);

    /* Factorise n into primes and exponents */
    idx_t primes[64], exps[64], len = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[len] = p;
            exps[len] = e;
            ++len;
        }
    }
    if (tmp > 1) {                     /* remaining prime factor */
        primes[len] = tmp;
        exps[len] = 1;
        ++len;
    }

    seq_t min_d = 0;
    dfs_divisors(0, 1, primes, exps, len, &min_d, s_n);
    if (min_d == 0) min_d = n;         /* safety net */

    return (seq_t)(n / min_d);
}
