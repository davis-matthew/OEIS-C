/*
 * A336363  Number of iterations of map k -> k*sigma(p^e)/p^e needed to reach
 *          a power of 2, where p is the largest prime factor of k and e is
 *          its exponent, when starting from k = n. a(n) = -1 if number of the
 *          form 2^k is never reached.
 * https://oeis.org/A336363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2670, u32 n <= 9879, u64 n <= 9879,
 *            u128 n <= 9879, big any n
 * Helpers:   is_power_of_two, largest_prime_factor, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 0 if n is a power of 2; otherwise
   a(n) = 1 + a(n * σ(pⁿ) / pⁿ) where pⁿ is the largest prime‑power dividing n.
   This function returns the number of steps needed to reach a power of 2,
   or -1 if it never occurs. */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    seq_t k = (seq_t)n;
    seq_t steps = 0;

    /* n ≥ 1 by definition of the sequence */
    while (!is_power_of_two(k)) {
        /* Find largest prime factor p of k (assumed to fit in uint64_t) */
        uint64_t kp = (uint64_t)k;
        uint64_t p = largest_prime_factor(kp);
        if (p <= 1) break;                    /* should not happen */

        /* Determine exponent e of p in k */
        uint64_t e = 0;
        seq_t t = k;
        while (t % (seq_t)p == 0) {
            t /= (seq_t)p;
            e++;
        }

        /* Compute m = p^e and σ(pⁿ) = (p^(e+1)-1)/(p-1) */
        seq_t m   = seq_pow((seq_t)p, (idx_t)e);
        seq_t num = seq_pow((seq_t)p, (idx_t)(e + 1));
        num -= 1;
        seq_t sigma = num / (seq_t)(p - 1);

        /* Advance to the next term */
        k = k * sigma / m;
        steps++;

        /* Avoid infinite loops: stop after a generous bound */
        if (steps > 10000) return (seq_t)(-1);
    }

    return steps;
}
