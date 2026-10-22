/*
 * A397620  a(n) = Sum_{d | n^3} d^3 * lambda(d) / Sum_{d | n^3} d *
 *          lambda(d), where lambda(n) denotes Liouville's function
 *          A008836(n).
 * https://oeis.org/A397620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   power_seq_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d | n^3} d^3 * lambda(d) / Sum_{d | n^3} d * lambda(d)
 *
 * The sequence is multiplicative, with a(p^e) = (1 + (-p)^(3*e+1) + p^(6*e+2)) / (1 - p + p^2).
 * This implementation uses that multiplicative formula: factor n, compute the prime‑power
 * contributions, and multiply them.
 *
 * For arbitrary partial exponents we use an exponentiation routine that works with
 * the `seq_t` type (no floating point or external libraries).  All operations use
 * only integer arithmetic; the result is exact whenever it fits in `seq_t`.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/power_seq_fast.h"

/* --- Helper: single prime‑power contribution a(p^e) --- */
static seq_t prime_power_term(idx_t p, idx_t e)
{
    seq_t p_seq = (seq_t)p;
    seq_t denom = p_seq * p_seq - p_seq + 1;          /* p^2 - p + 1 */

    /* exponents for the terms in the numerator */
    idx_t exp1 = 3 * e + 1;
    idx_t exp2 = 6 * e + 2;

    seq_t pow1 = power_seq_fast(p_seq, exp1);    /* p^(3e+1)  */
    seq_t pow2 = power_seq_fast(p_seq, exp2);    /* p^(6e+2)  */

    seq_t tmp = pow2 + (seq_t)1;                 /* 1 + p^(6e+2) */

    seq_t numerator;
    if ((e & 1) == 0) {                         /* e even → subtract */
        numerator = tmp - pow1;
    } else {                                    /* e odd  → add    */
        numerator = tmp + pow1;
    }

    return numerator / denom;                    /* integer division */
}

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset undefined; safe default */
    if (n == 1) return 1;            /* a(1) = 1 */

    seq_t result = 1;
    idx_t tmp = n;

    /* handle factor 2 separately for performance */
    idx_t e = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e) result *= prime_power_term(2, e);

    /* trial division for odd factors */
    for (idx_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            result *= prime_power_term(d, e);
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        result *= prime_power_term(tmp, 1);
    }

    return result;
}
