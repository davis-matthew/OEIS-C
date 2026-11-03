/* sigma_seq: the sum of the divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_SIGMA_SEQ_H
#define OEISC_UTILS_SIGMA_SEQ_H
#include <stdbool.h>
#include <stdint.h>

/* Compute σ(k) for a seq_t value using trial division. */
static seq_t sigma_seq(seq_t n)
{
    seq_t sum  = 1;
    seq_t temp = n;
    seq_t p    = 2;
    while (p * p <= temp) {
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t pow  = 1;
            while (temp % p == 0) {
                temp /= p;
                pow  *= p;
                term += pow;
            }
            sum *= term;
        }
        p += 1;
    }
    if (temp > 1) {
        sum *= (1 + temp);
    }
    return sum;
}

#endif
