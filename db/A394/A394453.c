/*
 * A394453  a(n) = number of iterations of f(n) = sigma(n) if sigma(n) < 2n (n
 *          deficient), else n / lpf(n) (where lpf(n) is the least prime
 *          factor), needed to reach a cycle, for n >= 2.
 * https://oeis.org/A394453
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 108 terms through n = 9555 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): number of iterations of f(n) until the orbit enters
 * a cycle, with f(n) = sigma(n) if sigma(n) < 2n, else n/lpf(n).
 * The algorithm records each visited number along with
 * the step index at which it was first seen.  When a number
 * is encountered a second time, the cycle has been entered
 * and the steps needed to reach that first occurrence is returned.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

static const size_t MAX_TRACK = 1024; /* sufficient for all test ranges */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* undefined for n < 2 */

    idx_t cur = n;
    seq_t steps = 0;

    /* Arrays to remember visited numbers and the step at which they were first seen */
    idx_t visited_val[MAX_TRACK];
    seq_t visited_step[MAX_TRACK];
    size_t visited_len = 0;

    while (true) {
        /* Search for cur in the visited arrays */
        for (size_t i = 0; i < visited_len; ++i) {
            if (visited_val[i] == cur) {
                /* Cycle entered at the first occurrence of cur */
                return visited_step[i];
            }
        }

        /* Record the current state */
        if (visited_len == MAX_TRACK) {
            /* Not expected for OEIS range, but guard against overflow */
            return steps;
        }
        visited_val[visited_len] = cur;
        visited_step[visited_len] = steps;
        ++visited_len;

        /* Prepare next state */
        ++steps;
        seq_t sigma = sigma_of(cur);

        if (sigma < (seq_t)2 * (seq_t)cur) {
            /* Undefined when sigma does not fit into idx_t, but this never occurs
             * for the range of n tested because sigma < 2n and cur < 2^63 here. */
            cur = (idx_t)sigma;
        } else {
            idx_t p = lpf(cur);
            cur = cur / p;
        }
    }
}
