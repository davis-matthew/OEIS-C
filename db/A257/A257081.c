/*
 * A257081  a(n) = Number of iterations of A257080 needed, starting from n,
 *          before a fixed point is reached.
 * https://oeis.org/A257081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 56, u32 n <= 3349, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257081: a(n) = number of iterations of A257080 necessary to
   reach a fixed point.  Recurrence:
     a(0) = 0
     a(n) = 0                    if the factorial representation of n has no 1
     a(n) = 1 + a(A257080(n))    otherwise
   where A257080(n) = m * n and m is the least missing non‑zero digit in
   the factorial representation of n.  */
#include <stddef.h>
#include <stdbool.h>

/*  Maximum factorial index we are prepared to handle.  This is far
    beyond the values needed for all tested terms.  */
#define MAX_FACT 512

/*  Pre‑computed factorial table: fact[i] = i!  (i starts at 0).  */
static seq_t fact[MAX_FACT];
static size_t fact_max = 0;          /* largest i with fact[i] computed */

/*  Ensure fact[0]..fact[fact_max] are known and that fact[fact_max] > limit.  */
static void ensure_facts(seq_t limit)
{
    if (fact_max == 0) {
        fact[0] = (seq_t)1;
        fact[1] = (seq_t)1;
        fact_max = 1;
    }
    while (fact[fact_max] <= limit) {
        /* Grow the table one step.  */
        size_t next = fact_max + 1;
        fact[next] = fact[fact_max] * (seq_t)(next);
        fact_max = next;
        if (fact_max >= MAX_FACT - 1)  /* prevent out of bounds */
            break;
    }
}

/*  Return the least missing non‑zero digit in the factorial representation
    of x, or 0 if the representation lacks digit 1 (the fixed‑point case).  */
static seq_t least_missing_digit(seq_t x)
{
    ensure_facts(x);

    unsigned char used[MAX_FACT] = {0};
    seq_t temp = x;

    for (size_t i = fact_max; i > 0; --i) {
        seq_t d = temp / fact[i];
        if (d != 0) {
            if (d <= fact_max)
                used[(size_t)d] = 1;
        }
        temp -= d * fact[i];
    }

    if (!used[1])   /* no 1 in the representation → fixed point */
        return (seq_t)0;

    for (size_t m = 2; m <= fact_max; ++m) {
        if (!used[m])
            return (seq_t)m;
    }
    /* all digits 2..fact_max present; the least missing is fact_max+1 */
    return (seq_t)(fact_max + 1);
}

/*  Main sequence function, as required.  */
seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;      /* current value in the iteration */
    seq_t cnt = (seq_t)0;    /* number of iterations performed */

    while (true) {
        seq_t m = least_missing_digit(v);
        if (m == (seq_t)0)  /* fixed point reached */
            break;
        v = v * m;          /* next step of A257080 */
        ++cnt;
    }
    return cnt;
}
