/* num_divisors_seq: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_SEQ_H
#define OEISC_UTILS_NUM_DIVISORS_SEQ_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* trial‑division divisor count. Works for values that fit in
 * seq_t and for the arbitrary‑precision case because the result
 * always fits in seq_t. */
static seq_t num_divisors_seq(seq_t x)
{
    if (x <= 1)
        return (seq_t)1;                /* d(1)=1, d(0) not used */

    seq_t count = 1;
    /* Count factor of 2. */
    unsigned exp = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        exp++;
    }
    if (exp > 0) {
        count *= (seq_t)(exp + 1);
    }

    /* Trial divide by odd numbers up to sqrt(x). */
    for (seq_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            count *= (seq_t)(exp + 1);
        }
    }

    if (x > 1)                /* remaining prime factor */
        count *= (seq_t)2;

    return count;
}

#endif
