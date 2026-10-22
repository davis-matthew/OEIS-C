/*
 * A374107  a(n) = 1 if A113177(n) and A328845(n) are both even, otherwise 0,
 *          where A113177 is fully additive with a(p) = Fibonacci(p) and
 *          A328845 is the first Fibonacci-based variant of the arithmetic
 *          derivative.
 * https://oeis.org/A374107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9781, u16 n <= 9781, u32 n <= 9781, u64 n <= 9781,
 *            u128 n <= 9781, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes whether A113177(n) and A328845(n) are both even;
   returns 1 if both even, otherwise 0.  Uses only parity
   information and simple trial division. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    uint64_t orig_n = (uint64_t)n;          /* keep original value */
    uint64_t m = orig_n;
    bool parity_a = false;  /* parity of A113177(n)  (false = even) */
    bool parity_d = false;  /* parity of A328845(n)  (false = even) */

    /* handle factor 2 separately */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) {
        /* Fib(2) = 1 is odd, so it contributes when e is odd */
        if ((e & 1ULL) != 0)
            parity_a ^= 1;

        /* For A328845: term parity = e*Fib(2)*(n/2) */
        if ((e & 1ULL) != 0 && ((orig_n / 2ULL) & 1ULL) != 0)
            parity_d ^= 1;
    }

    /* handle odd prime factors */
    for (uint64_t p = 3ULL; p <= m / p; p += 2ULL) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* Fib(p) is even iff p % 3 == 0 */
            bool fibodd = (p % 3ULL != 0ULL);

            if (fibodd && ((e & 1ULL) != 0))
                parity_a ^= 1;

            if (fibodd && ((e & 1ULL) != 0) && ((orig_n / p) & 1ULL) != 0)
                parity_d ^= 1;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1ULL) {
        uint64_t p = m;
        e = 1;
        bool fibodd = (p % 3ULL != 0ULL);

        if (fibodd)
            parity_a ^= 1;

        if (fibodd && ((orig_n / p) & 1ULL) != 0)
            parity_d ^= 1;
    }

    return (parity_a == false && parity_d == false) ? (seq_t)1 : (seq_t)0;
}
