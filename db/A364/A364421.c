/*
 * A364421  For n >= 3, r >= 0, y an integer, a(n) is the number of integral
 *          solutions to the elliptic equation y^2 = n^3 + n^2 + 2*r*n + r^2.
 * https://oeis.org/A364421
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 2426, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A364421: Number of integral solutions to y^2 = n^3 + n^2 + 2*r*n + r^2
 * Formula: For n ≥ 3, let n³ = ∏ p_i^{3·e_i} (factorization of n³).  
 * For each divisor d of n³ compute e = n³/d.  
 * If d > e, diff = d – e, and if diff ≥ 2n and diff is even, it corresponds
 * to a distinct integer r ≥ 0 solving the equation.  
 * a(n) is the count of such divisors d.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* factor_uint64: fills arrays pr, exp with prime factors of n and their exponents.
 * Returns the number of distinct prime factors.  Arrays must have space for at least 20 elements. */
static size_t factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp) {
    size_t cnt = 0;
    if (n <= 1) return 0;
    if ((n & 1ULL) == 0ULL) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0ULL) { n >>= 1; ++e; }
        pr[cnt] = 2ULL;  exp[cnt] = e;  ++cnt;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pr[cnt] = p;  exp[cnt] = e;  ++cnt;
        }
    }
    if (n > 1) { pr[cnt] = n;  exp[cnt] = 1;  ++cnt; }
    return cnt;
}

/* dfs_divs: recursively enumerate all divisors of n³ (represented by prime arrays) and
 * count those that satisfy the solution condition. */
static void dfs_divs(const uint64_t *pr, const uint64_t *exp, size_t idx,
                     size_t total, seq_t cur, seq_t n3, seq_t two_n,
                     seq_t *cnt) {
    if (idx == total) {
        seq_t d2 = n3 / cur;
        if (cur > d2) {                     /* only when d > n³/d */
            seq_t diff = cur - d2;
            if (diff >= two_n && ((diff & 1U) == 0U))
                ++(*cnt);
        }
        return;
    }
    uint64_t base  = pr[idx];
    uint64_t max_e = exp[idx] * 3ULL;        /* exponent in n³ */
    seq_t   val   = 1;
    for (uint64_t e = 0; e <= max_e; ++e) {
        seq_t new_cur = cur * val;
        dfs_divs(pr, exp, idx + 1, total, new_cur, n3, two_n, cnt);
        val *= base;
    }
}

seq_t a(idx_t n) {
    if (n < 3) return 0;                    /* defined from offset 3 */

    /* factor n */
    uint64_t primes[20];
    uint64_t exps  [20];
    size_t   fcnt  = factor_uint64((uint64_t)n, primes, exps);

    /* compute n³ and 2n in seq_t */
    seq_t n3   = (seq_t)n;
    n3 *= (seq_t)n;
    n3 *= (seq_t)n;
    seq_t two_n = (seq_t)(2 * (seq_t)n);

    /* count valid divisors */
    seq_t cnt = 0;
    dfs_divs(primes, exps, 0, fcnt, (seq_t)1, n3, two_n, &cnt);
    return cnt;
}
