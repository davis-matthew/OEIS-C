/*
 * A323596  Number of (positive) iterations of f to reach an integer when
 *          starting from n/1. If no integer is ever reached then a(n) = -1.
 *          f(p/q) = (p + q) / (A000120(p) + A000120(q)).
 * https://oeis.org/A323596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = number of (positive) f-iterations starting from n/1 until q==1; 
   f(p/q) = (p+q)/(popcount(p)+popcount(q)). Return -1 if the sequence never reaches an integer. */
#include <stdbool.h>

/* popcount of a seq_t value (number of 1-bits in binary representation) */
static inline seq_t popcount(seq_t x)
{
    seq_t c = 0;
    while (x) {
        x &= (x - 1);
        c += 1;
    }
    return c;
}

/* greatest common divisor for seq_t, Euclidean algorithm */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a < 0 ? -a : a;
}

/* one step of the function f: p/q -> (p+q)/(popcount(p)+popcount(q)) : reduced form */
static inline void step(seq_t p, seq_t q, seq_t *np, seq_t *nq)
{
    seq_t np_raw = p + q;
    seq_t nq_raw = popcount(p) + popcount(q);
    seq_t g = gcd_seq(np_raw, nq_raw);
    *np = np_raw / g;
    *nq = nq_raw / g;
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is defined for n >= 1 only */
        return -1;

    seq_t p = (seq_t)n;   /* start as n/1 */
    seq_t q = 1;

    /* first step (iteration 1) */
    seq_t tort_p, tort_q;
    step(p, q, &tort_p, &tort_q);
    seq_t steps = 1;
    if (tort_q == 1)
        return steps;

    /* second step for the hare (iteration 2) */
    seq_t hare_p, hare_q;
    step(p, q, &hare_p, &hare_q);
    step(hare_p, hare_q, &hare_p, &hare_q); /* advance two steps */
    /* hare is now at iteration 2 */

    while (true) {
        /* cycle detection: if tortoise equals hare, we have entered a cycle */
        if (tort_p == hare_p && tort_q == hare_q)
            return -1;

        /* advance tortoise by one step */
        step(tort_p, tort_q, &tort_p, &tort_q);
        steps += 1;
        if (tort_q == 1)
            return steps;

        /* advance hare by two steps */
        step(hare_p, hare_q, &hare_p, &hare_q);
        step(hare_p, hare_q, &hare_p, &hare_q);
    }
}
