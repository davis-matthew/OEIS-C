/*
 * A306345  Absolute difference between the number of prime divisors and the
 *          number of composite divisors of n.
 * https://oeis.org/A306345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |2 * (number of distinct prime factors of n) – (total
 *            number of divisors of n) + 1|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 1.  The function is thread‑safe and uses
   no global state.  It factors n by trial division, which is
   sufficient for the required input ranges (up to at least 10 000
   and well within the integer limits of seq_t). */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 1 */
    if (n == 1) {
        return 0;
    }

    uint64_t m = (uint64_t)n;          /* local copy for factoring   */
    int distinct_primes = 0;           /* A001221(n)                  */
    seq_t divisor_count = 1;           /* A000005(n)                  */

    /* Trial division up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            ++distinct_primes;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            divisor_count *= (seq_t)(exp + 1);
        }
    }

    /* If anything remains, it is a prime factor > sqrt(original n) */
    if (m > 1) {
        ++distinct_primes;
        divisor_count *= 2;
    }

    /* Apply the formula: a(n) = |2*distinct_primes – divisor_count + 1| */
    int64_t diff = (int64_t)2 * (int64_t)distinct_primes
                   - (int64_t)divisor_count + 1;
    return diff >= 0 ? (seq_t)diff : (seq_t)(-diff);
}
