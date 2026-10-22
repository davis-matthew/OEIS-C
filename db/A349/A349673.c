/*
 * A349673  a(n) is the smallest positive integer m such that the set of
 *          numbers {f(k) : 1 <= k <= n} are pairwise distinct modulo m for
 *          f(x)=x^3+x.
 * https://oeis.org/A349673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*3^(6s+4) if n = 3^(6s+5)+1 or n = 3^(6s+5)+2; *
 *            otherwise a(n) = 3^ceil(log_3(n))
 * Range:     u8 n <= 243, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>     /* for bool */

seq_t a(idx_t n)
{
    /* default: 3^ceil(log_3(n)) */
    seq_t ceil_pow = 1;
    idx_t  ceil_exp = 0;
    while (ceil_pow < (seq_t)n) {
        ceil_pow *= 3;
        ceil_exp++;
    }

    /* check special cases */
    bool special = false;
    idx_t special_exp = 0;  /* exponent e where 3^e = n-1 or n-2 */

    for (idx_t d = 1; d <= 2; ++d) {
        if (n <= d)                /* avoid unsigned underflow */
            continue;
        idx_t target = n - d;
        seq_t pow = 1;
        idx_t  exp = 0;
        while (pow < (seq_t)target) {
            pow *= 3;
            exp++;
        }
        if (pow == (seq_t)target && exp % 6 == 5) {
            special      = true;
            special_exp  = exp;   /* 3^special_exp == n-d, and special_exp ≡ 5 (mod 6) */
            break;
        }
    }

    if (special) {
        /* a(n) = 7 * 3^(special_exp-1)  (since special_exp = 6s+5) */
        seq_t pow = 1;
        for (idx_t i = 0; i < special_exp - 1; ++i)
            pow *= 3;
        return pow * (seq_t)7;
    } else {
        return ceil_pow;
    }
}
