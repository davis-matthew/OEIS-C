/*
 * A367168  The largest unitary divisor of n that is a term of A138302.
 * https://oeis.org/A367168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367168: largest unitary divisor of n that is a term of A138302.
   The sequence is multiplicative with a(p^e) = p^e if e is a power of 2,
   and a(p^e) = 1 otherwise.
   Hence a(n) = ∏_{p^e‖n, e power of 2} p^e. */

#include <stdint.h>
#include <stdbool.h>

/* internal helper: exponent e is a power of 2 iff e>0 and e&(e-1)==0 */
static inline bool is_power_of_two(idx_t e)
{
    return e > 0 && (e & (e - 1)) == 0;
}

/* internal helper: compute base^exp in seq_t, exp fits in idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1LL;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    if (n == 1)
        return res;

    /* factor out 2 separately for speed */
    idx_t e = 0;
    while ((n & 1ULL) == 0ULL)
    {
        n >>= 1;
        ++e;
    }
    if (e && is_power_of_two(e))
        res *= pow_seq((seq_t)2, e);

    /* odd prime factors */
    for (idx_t p = 3; p * p <= n; p += 2)
    {
        if (n % p == 0)
        {
            e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            if (e && is_power_of_two(e))
                res *= pow_seq((seq_t)p, e);
        }
    }

    /* remaining prime > sqrt(original n) */
    if (n > 1) /* exponent 1 */
        res *= pow_seq((seq_t)n, 1);

    return res;
}
