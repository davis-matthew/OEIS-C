/*
 * A381497  a(n) = sum of numbers k < n such that 1 < gcd(k,n) and rad(k) !=
 *          rad(n), where rad = A007947.
 * https://oeis.org/A381497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over 1 ≤ k < n with 1<gcd(k,n) and rad(k)!=rad(n) of
 *            k
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 79 terms through n = 9911 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static size_t factorize(uint64_t n, uint64_t primes[], int exps[])
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt] = e * 2;          /* exponents doubled for n² */
            cnt++;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 2;                /* exponent doubled */
        cnt++;
    }
    return cnt;
}

static seq_t rad(idx_t m)
{
    seq_t r = (seq_t)1;
    if (m < 2) return r;
    /* Handle factor 2 */
    if ((m & 1) == 0) {
        r *= (seq_t)2;
        while ((m & 1) == 0) m >>= 1;
    }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            r *= (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= (seq_t)m;  /* remaining prime factor */
    return r;
}

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

static void dfs_same_rad(int idx, idx_t m, idx_t limit,
                         const uint64_t *primes, int pf_cnt,
                         seq_t rad_seq, seq_t *sum)
{
    if (m > limit)
        return;
    if (idx == pf_cnt) {
        *sum += rad_seq * (seq_t)m;
        return;
    }
    idx_t p = (idx_t)primes[idx];
    idx_t cur = m;
    for (;;) {
        dfs_same_rad(idx + 1, cur, limit, primes, pf_cnt, rad_seq, sum);
        if (cur > limit / p)
            break;
        cur *= p;
    }
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t total_sum = (n_seq * (n_seq - 1)) / 2;   /* sum_{k=1}^{n-1} k */
    seq_t phi_n   = phi(n);
    seq_t sum_coprime = n_seq * phi_n / 2;         /* sum of k coprime to n */
    seq_t sum_gcd = total_sum - sum_coprime;       /* sum of k with gcd>1 */

    seq_t rad_seq = rad(n);
    idx_t rad_val = (idx_t)rad_seq;
    idx_t limit = (n - 1) / rad_val;               /* maximal m with rad*m < n */

    seq_t sum_same_rad = 0;
    if (limit > 0) {
        uint64_t primes[20];
        int exps[20];
        int cnt = factorize(n, primes, exps);
        dfs_same_rad(0, 1, limit, primes, cnt, rad_seq, &sum_same_rad);
    }

    return sum_gcd - sum_same_rad;
}
