/*
 * A360577  Number of 3-dimensional tilings of a 2 X 2 X n box using 2 X 2 X 1
 *          plates, 2 X 1 X 1 dominos and trominos (L-shaped connection of 3
 *          cubes).
 * https://oeis.org/A360577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence for A360577 (from the OEIS entry):
   a(n) = 3*a(n-1) + b(n-1) + c(n-1)
          + (n>=2 ? 19*a(n-2) + 4*b(n-2) + c(n-2) + 2*d(n-2) : 0)
   b(n) = 12*a(n-1) + 2*b(n-1) + 2*c(n-1) + e(n-1)
   c(n) = 20*a(n-1) + 6*b(n-1) + 2*c(n-1) + 2*e(n-1)
   d(n) = 4*a(n-1)  + 2*b(n-1) + d(n-1)
   e(n) = 24*a(n-1) + 7*b(n-1) + 2*c(n-1) + 2*d(n-1)
   with a(0)=1, b(0)=c(0)=d(0)=e(0)=0.  The first term a(0)=1 is the
   offset of the sequence. */

/* A state holds the five auxiliary values for a given index. */
typedef struct {
    seq_t a, b, c, d, e;
} State;

/* Dynamic array of states. */
static State *states = NULL;
static size_t cap      = 0;   /* array capacity in elements */
static size_t cur_max  = 0;   /* highest computed index */

/* Ensure that the states array can hold indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n < cap) return;

    size_t new_cap = cap > 0 ? cap << 1 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    State *tmp = (State *)realloc(states, new_cap * sizeof(*states));
    if (!tmp) abort();      /* allocation failure */
    states = tmp;
    cap = new_cap;
}

/* Initialise the table for index 0. */
static void
initialise(void)
{
    if (cap == 0) {
        ensure_capacity(0);
        states[0].a = 1;
        states[0].b = 0;
        states[0].c = 0;
        states[0].d = 0;
        states[0].e = 0;
        cur_max = 0;
    }
}

/* Return the n-th term of the sequence. */
seq_t
a(idx_t n)
{
    initialise();

    /* Return stored value if already computed. */
    if (n <= cur_max) return states[(size_t)n].a;

    /* Compute all missing values up to n. */
    ensure_capacity(n);

    for (size_t i = cur_max + 1; i <= (size_t)n; ++i) {
        /* Previous values */
        seq_t a_prev  = states[i - 1].a;
        seq_t b_prev  = states[i - 1].b;
        seq_t c_prev  = states[i - 1].c;
        seq_t d_prev  = states[i - 1].d;
        seq_t e_prev  = states[i - 1].e;

        /* a_i */
        seq_t term1 = 3 * a_prev + b_prev + c_prev;
        seq_t term2 = 0;
        if (i >= 2) {
            seq_t a_prev2 = states[i - 2].a;
            seq_t b_prev2 = states[i - 2].b;
            seq_t c_prev2 = states[i - 2].c;
            seq_t d_prev2 = states[i - 2].d;
            term2 = 19 * a_prev2 + 4 * b_prev2 + c_prev2 + 2 * d_prev2;
        }
        seq_t a_i = term1 + term2;

        /* b_i, c_i, d_i, e_i */
        seq_t b_i = 12 * a_prev + 2 * b_prev + 2 * c_prev + e_prev;
        seq_t c_i = 20 * a_prev + 6 * b_prev + 2 * c_prev + 2 * e_prev;
        seq_t d_i = 4 * a_prev + 2 * b_prev + d_prev;
        seq_t e_i = 24 * a_prev + 7 * b_prev + 2 * c_prev + 2 * d_prev;

        /* Store the new state */
        states[i].a = a_i;
        states[i].b = b_i;
        states[i].c = c_i;
        states[i].d = d_i;
        states[i].e = e_i;
    }

    cur_max = (size_t)n;
    return states[(size_t)n].a;
}
