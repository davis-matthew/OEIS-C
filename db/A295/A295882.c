/*
 * A295882  Balanced ternary representation of the deficiency of n,
 *          A033879(n).
 * https://oeis.org/A295882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with d = A033879(n) = 2n - sigma(n), a(n) = A117967(d) if d >=
 *            0, else A117968(-d); i.e. write d in balanced ternary and
 *            replace each digit -1 with 2
 * Range:     u8 n <= 125, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9784 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t j = n / i;
            sum += (seq_t)i;
            if (i != j)
                sum += (seq_t)j;
        }
    }
    seq_t two_n = (seq_t)2 * (seq_t)n;
    int neg = 0;
    seq_t m;
    if (two_n >= sum) {
        m = two_n - sum;
    } else {
        m = sum - two_n;
        neg = 1;
    }
    seq_t res = 0;
    seq_t p = 1;
    while (m > 0) {
        int r = (int)(m % 3);
        int dg;
        if (r == 0) {
            dg = 0;
            m /= 3;
        } else if (r == 1) {
            dg = 1;
            m = (m - 1) / 3;
        } else {
            dg = -1;
            m = (m + 1) / 3;
        }
        if (neg)
            dg = -dg;
        if (dg == 1)
            res += p;
        else if (dg == -1)
            res += p * 2;
        p *= 3;
    }
    return res;
}
