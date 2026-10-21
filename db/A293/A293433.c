/*
 * A293433  a(n) is the number of the proper divisors of n that are Jacobsthal
 *          numbers (A001045).
 * https://oeis.org/A293433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of proper divisors of n that are Jacobsthal
 *            numbers (A001045)
 * Range:     u8 n <= 140, u16 n <= 9558, u32 n <= 9558, u64 n <= 9558,
 *            u128 n <= 9558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Count the divisor 1, if it is proper */
    if (n > 1 && (n % 1 == 0))
        ++count;

    /* Generate Jacobsthal numbers and count those that divide n */
    seq_t prev2 = 0; /* J(0) */
    seq_t prev1 = 1; /* J(1) */
    seq_t last_j = prev1;  /* remember last distinct Jac number */
    seq_t jnext;

    while (1) {
        jnext = prev1 + ((seq_t)2 * prev2);
        if (jnext > (seq_t)n)
            break;
        if (jnext != last_j) {          /* skip duplicate 1 */
            if ((((seq_t)n > jnext) && (((seq_t)n % jnext) == 0)))
                ++count;
            last_j = jnext;
        }
        prev2 = prev1;
        prev1 = jnext;
    }

    return count;
}
