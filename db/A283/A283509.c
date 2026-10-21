/*
 * A283509  Remainder when sum of first n terms of the Hofstadter Q-sequence
 *          is divided by 2*n.
 * https://oeis.org/A283509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{k=1..n} A005185(k) ) mod (2*n)
 * Range:     u8 n <= 28, u16 n <= 984, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A005185(k) is the Hofstadter Q‑sequence */

#include <stdlib.h>
#include <stdint.h>

static seq_t *Q = NULL;      /* 1‑indexed array of Q‑values (Q[1]=1,Q[2]=1) */
static idx_t Qlen = 0;       /* largest index that has been computed */

/* Ensure that Q[1..n] are computed; grow the array if necessary. */
static void
ensure_Q(idx_t n)
{
    if (n <= Qlen) return;          /* already ready */

    /* Allocate space for indices up to n (plus index 0 for 1‑based). */
    idx_t newlen = n + 1;
    seq_t *tmp = (seq_t *)realloc(Q, (size_t)newlen * sizeof(*Q));
    if (!tmp) abort();              /* out of memory */
    Q = tmp;

    for (idx_t i = Qlen + 1; i <= n; ++i) {
        if (i == 1) {
            Q[1] = 1;
        } else if (i == 2) {
            Q[2] = 1;
        } else {
            /* Q(i) = Q(i−Q(i−1)) + Q(i−Q(i−2)) */
            idx_t a = i - (idx_t)Q[i - 1];
            idx_t b = i - (idx_t)Q[i - 2];
            Q[i] = Q[a] + Q[b];
        }
    }
    Qlen = n;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0; return 0 */

    /* Compute Q up to n. */
    ensure_Q(n);

    /* Sum the first n Q values.  For all n≤10000 the sum fits in
       any 64‑ or 128‑bit type; with arbitrary‑precision it obviously
       fits in seq_t. */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        sum += Q[i];
    }

    /* Compute the remainder modulo 2*n. */
    seq_t divisor = ((seq_t)n) * 2;   /* 2*n fits in seq_t for all n tested */
    return sum % divisor;
}
