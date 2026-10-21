/*
 * A298410  Unique least common multiples for {1,2,...,n}.
 * https://oeis.org/A298410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A298410:
 * a(n) is the LCM of 1..m where m is the n‑th integer such that
 * m and m+1 are distinct prime powers.  The value is produced
 * incrementally by checking each integer in order and multiplying
 * the current LCM by its base prime whenever a new prime power
 * is reached.  The algorithm is O(m_max) where m_max is the
 * m corresponding to the desired term, typically a few times
 * the requested n.  It runs fast for n up to 10 000.
 */
#include <stdbool.h>
#include <stdint.h>

/* Test whether n is a prime power.  If true, *base is set to the
   prime base of the power.  Returns false for n <= 1.  */
static bool
is_prime_power(unsigned long long n, unsigned long long *base)
{
    if (n <= 1)
        return false;
    unsigned long long temp = n;

    /* handle factor 2 separately to allow i+=2 loop */
    if (temp % 2 == 0) {
        *base = 2;
        while (temp % 2 == 0)
            temp /= 2;
        return temp == 1;
    }
    for (unsigned long long p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            *base = p;
            while (temp % p == 0)
                temp /= p;
            return temp == 1;
        }
    }
    /* remaining factor is a prime > sqrt(original n) */
    *base = temp;
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    unsigned long long m = 1;   /* previous integer */
    seq_t cur = 1;              /* LCM(1..(m)) */

    for (;;) {
        ++m;  /* consider next integer */

        unsigned long long base_m, base_next;
        bool pp_m = is_prime_power(m, &base_m);

        if (pp_m) {
            /* new power of prime base_m reached – multiply LCM */
            cur = cur * (seq_t)base_m;
        }

        bool pp_next = is_prime_power(m + 1, &base_next);

        if (pp_m && pp_next && base_m != base_next) {
            ++count;
            if (count == n)
                return cur;   /* a(n) found */
        }
    }
}
