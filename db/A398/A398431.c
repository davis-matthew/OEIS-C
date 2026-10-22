/*
 * A398431  a(n) is the least positive multiple of n that is a factorial base
 *          Niven number.
 * https://oeis.org/A398431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A398496(n) * n, where A398496(n) is the least integer k
 *            so that k*n is a factorial base Niven number
 * Range:     u8 n <= 28, u16 n <= 1288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the sum of the digits of x in the factorial number system. */
static seq_t factorial_digit_sum(seq_t x)
{
    seq_t sum = (seq_t)0;
    seq_t temp = x;
    seq_t i = 1;                 /* i corresponds to the factorial index */
    while (temp > (seq_t)0)
    {
        seq_t base = i + (seq_t)1;   /* next divisor (i+1) */
        seq_t d = temp % base;      /* current digit d_i */
        sum += d;
        temp = temp / base;          /* remove the contribution of this digit */
        ++i;
    }
    return sum;
}

/* Sequence A398431: the least positive multiple of n that is a factorial
   base Niven number. */
seq_t a(idx_t n)
{
    if (n <= (idx_t)0)   /* safety for out‑of‑range indices */
        return (seq_t)0;

    const seq_t nseq = (seq_t)n;   /* promote index to sequence type   */

    /* Iterate over multiples of n until Niven condition satisfied. */
    for (idx_t k = (idx_t)1; ; ++k)
    {
        const seq_t m = nseq * (seq_t)k;            /* current multiple */
        const seq_t sum = factorial_digit_sum(m);   /* digit sum in factorial base */

        /* The sum will never be 0 for a positive m, but guard just in case. */
        if (sum == (seq_t)0)
            continue;

        if (m % sum == (seq_t)0)          /* Niven condition satisfied */
            return m;
    }
}
