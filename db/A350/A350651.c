/*
 * A350651  a(n) is the size of the orbit of n under repeated application of
 *          A350229 (the sum of a number and its balanced ternary digits).
 * https://oeis.org/A350651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350651
    a(n) = size of the orbit of n under repeated application of
    f(x) = x + sum of the digits of x in balanced ternary.
    The orbit is the set { f^k(n) | k >= 0 } until a value repeats.

    This implementation uses Floyd's cycle‑finding algorithm, so no
    extra memory is required beyond a few temporary seq_t variables.  */

#include <stddef.h>  /* for size_t */

/* Return f(v) = v + sum of digits of v in balanced ternary. */
static seq_t f_func(seq_t v)
{
    long long sum = 0;            /* signed accumulator for digit sum */
    seq_t tmp = v;

    /* Process digits in base‑3, interpreting 2 as the digit -1 and
       carrying one to the next higher digit. */
    while (tmp > 0) {
        seq_t r = tmp % 3;
        if (r == 2) {            /* digit -1 with carry */
            sum -= 1;
            tmp = tmp / 3 + 1;
        } else {                 /* digit 0 or 1 */
            sum += (long long)r;
            tmp = tmp / 3;
        }
    }

    /* Compute v + sum, where sum may be negative.  Since v ≥ |sum|,
       the result is always non‑negative. */
    seq_t result = v;
    if (sum >= 0)
        result += (seq_t)sum;
    else
        result -= (seq_t)(-sum);
    return result;
}

/*  a(n) : number of distinct values in the orbit of n under f_func. */
seq_t a(idx_t n)
{
    seq_t x0 = (seq_t)n;          /* starting value (n itself) */

    /* --- Floyd's cycle–finding -- */
    seq_t tortoise = f_func(x0);
    seq_t hare = f_func(f_func(x0));
    while (tortoise != hare) {
        tortoise = f_func(tortoise);
        hare     = f_func(f_func(hare));
    }

    /* --- find μ (pre‑period length) --- */
    size_t mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = f_func(tortoise);
        hare     = f_func(hare);
        mu++;
    }

    /* --- find λ (cycle length) --- */
    size_t lambda = 1;
    hare = f_func(tortoise);
    while (tortoise != hare) {
        hare = f_func(hare);
        lambda++;
    }

    /* Total distinct values = μ + λ. */
    return (seq_t)(mu + lambda);
}
