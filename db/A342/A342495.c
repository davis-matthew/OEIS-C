/*
 * A342495  Number of compositions of n with constant (equal) first quotients.
 * https://oeis.org/A342495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A342495: Number of compositions of n with constant first quotients.
    a(n) = number of ordered partitions of n such that all successive
    quotients are equal.                                                */
#include <stdint.h>
#include <stdbool.h>

/* integer gcd for small arguments */
static inline int gcd_int(int a, int b)
{
    while (b) { int t = b; b = a % b; a = t; }
    return a;
}

/* power with early cutoff: returns base^exp if ≤ limit, otherwise limit+1 */
static seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (res > limit / base) return limit + 1;
        res *= base;
    }
    return res;
}

/* geometric sum s = Σ_{j=0}^{k-1} p^j q^{k-1-j}.
   If s > limit, returns limit+1.  */
static seq_t geom_sum(idx_t k, seq_t p, seq_t q, seq_t limit)
{
    seq_t sum = 0;
    for (idx_t j = 0; j < k; ++j) {
        seq_t p_pow = pow_limit(p, j, limit);
        if (p_pow > limit) return limit + 1;
        seq_t q_pow = pow_limit(q, k - 1 - j, limit);
        if (q_pow > limit) return limit + 1;
        seq_t term = p_pow * q_pow;
        if (term > limit) return limit + 1;
        if (sum > limit - term) return limit + 1;
        sum += term;
    }
    return sum;
}

/* divisor count of n (n>0). n==0 is handled separately in a(). */
static seq_t divisor_count(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 by definition */
    seq_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i == n) ++cnt;
            else cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */
    seq_t ans = divisor_count(n);       /* constant‑ratio (r = 1) compositions */
    const seq_t limit = (seq_t)n;        /* used for early cutoff */

    /* k = 1 already counted in divisors.   Enumerate k ≥ 2. */
    for (idx_t k = 2; ; ++k) {
        /* minimal s for this k is 2^k - 1 (p = 2, q = 1 or vice‑versa) */
        seq_t pow2k = pow_limit(2, k, limit + 1);
        if (pow2k > limit + 1) break;   /* even the smallest sum > n */

        for (idx_t q = 1; ; ++q) {
            /* smallest s for this q (p = 1 or 2) */
            seq_t p_min = (q == 1 ? 2 : 1);
            seq_t s_min = geom_sum(k, p_min, q, limit);
            if (s_min > limit) break;   /* further q will be even larger */

            for (idx_t p = 1; ; ++p) {
                if (p == q) continue;
                if (gcd_int((int)p, (int)q) != 1) continue;
                seq_t s = geom_sum(k, (seq_t)p, (seq_t)q, limit);
                if (s > limit) break;   /* further p will only increase s */
                if (limit % s == 0) ++ans;  /* m = n / s gives a valid composition */
            }
        }
    }
    return ans;
}
