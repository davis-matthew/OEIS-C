/*
 * A248197  Least positive integer m such that m + n divides prime(prime(m)) +
 *          prime(prime(n)).
 * https://oeis.org/A248197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive m such that (m + n) divides
 *            prime(prime(m)) + prime(prime(n)), where prime(k) is the k‑th
 *            prime (prime(1)=2)
 * Range:     u8 n <= 4, u16 n <= 581, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

static inline seq_t prime_of_prime(idx_t k)
{
    /* k >= 1; get_prime returns the k-th prime (uint32_t) */
    uint32_t p1 = get_prime(k);      /* 1st prime index -> 2, etc. */
    uint32_t p2 = get_prime(p1);     /* second prime index */
    return (seq_t)p2;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index; return minimal positive m */
    if (n == 0) return 0;   /* undefined, but prevents accidental use */

    seq_t pp_n = prime_of_prime(n);

    for (idx_t m = 1; ; ++m)
    {
        seq_t pp_m = prime_of_prime(m);
        seq_t sum   = pp_m + pp_n;
        seq_t div   = (seq_t)(n + m);

        if ((sum % div) == 0)
            return (seq_t)m;
    }
}
