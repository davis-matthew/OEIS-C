/*
 * A337819  a(n) is the smallest number k for which k*d is a Niven number, for
 *          any divisor d of n, n >= 1.
 * https://oeis.org/A337819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A337819: a(n) = smallest integer k such that k*d is a
 * and Niven number (divisible by the sum of its decimal digits) for
 * every divisor d of n, n ≥ 1.
 */
 
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* collect divisors of n in a small stack array */
    idx_t divs[200];
    idx_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            if (i != n / i)
                divs[cnt++] = n / i;
        }
    }

    seq_t k = 1;
    for (;;) {
        bool ok = true;
        for (idx_t j = 0; j < cnt; ++j) {
            idx_t d = divs[j];
            seq_t prod = k * (seq_t)d;
            seq_t ds   = digit_sum(prod);
            if ((uint64_t)(prod % ds) != 0) { /* modulo must be zero */
                ok = false;
                break;
            }
        }
        if (ok)
            return k;
        ++k;
    }
}
