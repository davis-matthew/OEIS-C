/*
 * A373493  a(n) = 1 if A059975(n) and A003415(n) are both multiples of 3,
 *          otherwise 0, where A059975 is fully additive with a(p) = p-1, and
 *          A003415 is the arithmetic derivative.
 * https://oeis.org/A373493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3355, u16 n <= 9071, u32 n <= 9071, u64 n <= 9071,
 *            u128 n <= 9071, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if A059975(n) and A003415(n) are both multiples of 3, else 0.
 * A059975(n) is fully additive with a(p)=p-1, i.e. sum e(p)*(p-1) over all
 * prime factors of n with multiplicity.
 * A003415(n) is the arithmetic derivative of n, which equals
 *    n * sum e(p)/p  for n=∏p^e(p).
 * Since n int-cast to seq_t, this works for any seq_t exceeding the size
 * of n.  The implementation factors n by trial division, suitable for n≤10⁴
 * (and fast enough for much larger n in a single call). */
#include <stdint.h>
#include <stdbool.h>

/* Factor n by trial division; returns the smallest prime factor.
 * Not exported; local helper. */
static uint64_t _spf(uint64_t n)
{
    if (n % 2 == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2)
        if (n % p == 0) return p;
    return n;  /* n is prime */
}

seq_t a(idx_t n)
{
    /* n>=1 by sequence definition */
    if (n == 0) return (seq_t)0;  /* undefined below offset */

    seq_t add_sum = 0;   /* A059975(n) */
    seq_t deriv_sum = 0; /* A003415(n) */
    const seq_t n_val = (seq_t)n;

    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t p = _spf(m);
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        add_sum += (seq_t)exp * ((seq_t)p - 1);
        deriv_sum += (seq_t)exp * (n_val / (seq_t)p);
    }

    bool add_mult3  = (add_sum % (seq_t)3) == 0;
    bool deriv_mult3 = (deriv_sum % (seq_t)3) == 0;

    return (add_mult3 && deriv_mult3) ? (seq_t)1 : (seq_t)0;
}
