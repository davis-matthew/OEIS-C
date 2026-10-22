/*
 * A306264  a(n) = 1 + d*a(n/d); a(1)=0. If n has only one prime divisor, then
 *          d=n, otherwise d is the greatest proper unitary divisor of n.
 * https://oeis.org/A306264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 509, u16 n <= 9682, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A306264:
 *   a(1) = 0
 *   for n > 1:
 *       let m be the smallest prime power divisor of n
 *       if n is a prime power (i.e. only one distinct prime factor), return 1
 *       otherwise let d = n / m (the largest proper unitary divisor of n)
 *       a(n) = 1 + d * a(m)
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    idx_t temp = n;
    idx_t min_power = n;      /* current smallest prime power observed */
    int distinct = 0;          /* number of distinct prime factors */

    /* trial division to factor n and find minimal prime power */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            ++distinct;
            idx_t power = 1;
            while (temp % p == 0) {
                power *= p;
                temp /= p;
            }
            if (power < min_power)
                min_power = power;
        }
    }
    /* remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        ++distinct;
        if (temp < min_power)
            min_power = temp;
    }

    /* if only one distinct prime factor: prime power =>
       greatest proper unitary divisor is n itself */
    if (distinct == 1)
        return 1;

    /* recursive call on the smallest prime power */
    idx_t d = n / min_power;   /* largest proper unitary divisor */
    seq_t val = a(min_power);
    return (seq_t)1 + (seq_t)d * val;
}
