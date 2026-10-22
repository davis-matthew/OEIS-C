/*
 * A323356  For a rational number p/q let f(p/q) = (p+q) / (A000120(p) +
 *          A000120(q)); a(n) is obtained by iterating f, starting at n/1,
 *          until an integer is reached (and then a(n) = that integer), or if
 *          no integer is ever reached then a(n) = -1.
 * https://oeis.org/A323356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 55, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for a rational p/q define
 *   f(p/q) = (p+q) / (popcount(p)+popcount(q))
 * Starting from n/1, iterate f until the denominator becomes 1.
 * The reached integer is a(n); if the iteration enters a cycle and
 * never reaches an integer, a(n) = -1.  This implements OEIS A323356. */

#include <stddef.h>
#include "../utils/gcd.h"

/* count ones in the binary representation of a seq_t value */
static seq_t popcount_seq_t(seq_t x)
{
    seq_t c = 0;
    while (x)
    {
        c += (x & 1);
        x >>= 1;
    }
    return c;
}

/* apply one step of the map to the fraction p/q */
static void step(seq_t *p, seq_t *q)
{
    seq_t num = *p + *q;
    seq_t den = popcount_seq_t(*p) + popcount_seq_t(*q);
    seq_t g   = gcd(num, den);
    *p = num / g;
    *q = den / g;
}

/* sequence A323356 */
seq_t a(idx_t n)
{
    /* start from n/1 */
    seq_t tort_p = (seq_t)n, tort_q = 1;
    seq_t hare_p = tort_p,          hare_q = 1;

    /* Floyd's cycle‑finding algorithm */
    while (1)
    {
        /* hare moves two steps */
        step(&hare_p, &hare_q);
        if (hare_q == 1) return hare_p;
        step(&hare_p, &hare_q);
        if (hare_q == 1) return hare_p;

        /* tortoise moves one step */
        step(&tort_p, &tort_q);
        if (tort_q == 1) return tort_p;

        /* cycle detection */
        if (tort_p == hare_p && tort_q == hare_q)
            return (seq_t)-1;
    }
}
