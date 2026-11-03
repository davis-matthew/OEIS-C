/* num_divisors_i32: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_I32_H
#define OEISC_UTILS_NUM_DIVISORS_I32_H
#include <stdbool.h>
#include <stdint.h>

/* compute tau(e) for a small exponent e (e >= 1) */
static seq_t num_divisors_i32(int e)
{
    seq_t res = 1;
    int i = 2;
    while (i * i <= e) {
        int cnt = 0;
        while (e % i == 0) {
            e /= i;
            cnt++;
        }
        if (cnt > 0) {
            res *= (cnt + 1);
        }
        i++;
    }
    if (e > 1) {          /* remaining prime factor */
        res *= 2;
    }
    return res;
}

#endif
