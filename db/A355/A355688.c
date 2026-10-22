/*
 * A355688  Dirichlet inverse of A354354, characteristic function of numbers
 *          that are neither multiples of 2 nor 3.
 * https://oeis.org/A355688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 9636, i16 n <= 9636, i32 n <= 9636, i64 n <= 9636,
 *            i128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A355688: Dirichlet inverse of A354354 (characteristic function of numbers coprime to 6)
 * Formula:  a(1) = 1
 *           For n > 1,
 *                a(n) = 0                      if n is divisible by 2 or 3,
 *                a(n) = (-1)^ω(n)               otherwise,
 *            where ω(n) = number of distinct prime divisors of n
 *            (all primes > 3).
 * This follows from the multiplicative definition:
 *    a(p^e) = 0 if p <= 3 or e >= 2,  a(p) = -1 for primes p > 3.
 *
 * The routine applies a simple trial‑division factorisation of n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)           /* a(1) = 1 */
        return (seq_t)1;

    /* divisible by 2 or 3 → zero */
    if ((n & 1ULL) == 0ULL || (n % 3ULL) == 0ULL)
        return (seq_t)0;

    uint64_t m = n;          /* local copy for factorisation */
    int    prime_cnt = 0;    /* number of distinct primes (>3) */

    /* trial division by primes of the form 6k ± 1 */
    for (uint64_t p = 5ULL; p * p <= m; p += 6ULL) {
        if (m % p == 0ULL) {
            int exp = 0;
            while (m % p == 0ULL) {
                ++exp;
                m /= p;
                if (exp > 1)          /* square factor → μ(m)=0 */
                    return (seq_t)0;
            }
            ++prime_cnt;
        }
        uint64_t q = p + 2ULL;      /* 6k + 1 */
        if (q * q <= m && m % q == 0ULL) {
            int exp = 0;
            while (m % q == 0ULL) {
                ++exp;
                m /= q;
                if (exp > 1)
                    return (seq_t)0;
            }
            ++prime_cnt;
        }
    }

    /* any remaining prime factor (> sqrt original n) */
    if (m > 1ULL) {
        ++prime_cnt;
    }

    /* a(n) = (-1)^prime_cnt */
    return ((prime_cnt & 1) ? (seq_t)-1 : (seq_t)1);
}
