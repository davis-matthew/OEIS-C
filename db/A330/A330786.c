/*
 * A330786  Number of steps to reach 1 by iterating the absolute alternating
 *          sum-of-divisors function (A206369).
 * https://oeis.org/A330786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0; for n > 1, a(n) = 1 + a(A206369(n)).
   The implementation iteratively applies the absolute alternating
   sum-of-divisors function A206369 until reaching 1. */

#include <stdint.h>

/* Helper: absolute alternating sum of divisors (A206369) of n.
   The result is returned as seq_t. */
static seq_t abs_alt_sum_divisors(idx_t n)
{
    if (n == 1) {
        /* For n = 1, the only divisor is 1, Ω(1)=0 (even), so the
           absolute alternating sum is |+1| = 1. */
        return (seq_t)1;
    }

    /* Factor n by trial division. For n ≤ 10000 this is trivial. */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[20];
    unsigned   exps[20];
    unsigned   pc   = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[pc] = tmp;
        exps[pc]   = 1;
        ++pc;
    }

    /* Enumerate all divisors and record their Ω values. */
    uint64_t divs[512];
    unsigned omegas[512];
    unsigned size = 1;      /* current number of divisors */
    divs[0]    = 1;
    omegas[0]  = 0;

    for (unsigned pi = 0; pi < pc; ++pi) {
        uint64_t p = primes[pi];
        unsigned e = exps[pi];
        unsigned curSize = size;   /* snapshot before adding new divisors */
        for (unsigned i = 0; i < curSize; ++i) {
            uint64_t base        = divs[i];
            unsigned baseOmega   = omegas[i];
            uint64_t val         = base;
            for (unsigned k = 1; k <= e; ++k) {
                val *= p;
                if (size < 512) {
                    divs[size]   = val;
                    omegas[size] = baseOmega + k;
                    ++size;
                }
            }
        }
    }

    /* Compute the alternating sum. */
    long long sum = 0;
    for (unsigned i = 0; i < size; ++i) {
        if ((omegas[i] & 1U) == 0) {
            sum += (long long)divs[i];
        } else {
            sum -= (long long)divs[i];
        }
    }
    if (sum < 0) sum = -sum;   /* absolute value */

    return (seq_t)sum;
}

/* The sequence function as required by the prompt. */
seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* by definition */

    seq_t steps = 0;
    idx_t cur = n;

    while (cur != 1) {
        seq_t f = abs_alt_sum_divisors(cur);

        /* Convert f back to idx_t for the next iteration. For
           C++ compilation we use static_cast, which is also
           acceptable in C when the class has a suitable
           conversion operator. */
#ifdef __cplusplus
        cur = static_cast<idx_t>(f);
#else
        cur = (idx_t)f;
#endif

        steps++;
    }

    return steps;
}
