/*
 * A281760  Number of n X 3 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A281760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) for
 *            n>11, with initial values: a(1)=2, a(2)=14, a(3)=47, a(4)=90,
 *            a(5)=201, a(6)=374, a(7)=672, a(8)=1172, a(9)=2015, a(10)=3442,
 *            a(11)=5859
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 37, u64 n <= 81, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[12] = {
    (seq_t)0,                    /* unused 0‑th element */
    (seq_t)2,  (seq_t)14, (seq_t)47, (seq_t)90, (seq_t)201,
    (seq_t)374, (seq_t)672, (seq_t)1172, (seq_t)2015,
    (seq_t)3442, (seq_t)5859
};

static seq_t *seq = NULL;   /* 1‑based array of computed terms   */
static idx_t  seq_len = 0;  /* highest index already computed   */

/* Compute a(n) using the recurrence.  Assumes seq[1..seq_len] are valid. */
static void compute_up_to(idx_t n)
{
    if (seq_len >= n)
        return;                  /* already computed */

    /* Allocate (or grow) array for indices 0..n */
    size_t needed = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(seq, needed * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;

    /* Fill missing terms */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        if (i <= 11) {
            seq[i] = init[i];
        } else {
            seq_t t = 4 * seq[i-1];
            t -= 4 * seq[i-2];
            t -= 2 * seq[i-3];
            t += 4 * seq[i-4];
            t -= seq[i-6];
            seq[i] = t;
        }
    }
    seq_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n=0, but return 0 for safety */
    compute_up_to(n);
    return seq[n];
}
