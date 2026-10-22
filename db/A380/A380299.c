/*
 * A380299  Sequence of primitive Pythagorean triples beginning with the
 *          triple (3,4,5), with each subsequent triple having as its inradius
 *          the area of the previous triple, and with the long leg and the
 *          hypotenuse of each triple being consecutive natural numbers.
 * https://oeis.org/A380299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A380299.  Recurrence:
   a(1) = 3.
   For k ≥ 1 let x = a(3k-2).
   a(3k-1)  = (x*x - 1)/2
   a(3k)    = a(3k-1) + 1
   a(3k+1)  = x * a(3k-1) + 1
   After computing the triple set, set x = a(3k+1) for the next step.
   This generates 3,4,5;13,84,85;1093,597324,597325;… */

#include <stdlib.h>

static seq_t *cache = NULL;   /* store computed terms (0‑based)      */
static size_t cap  = 0;       /* current allocation size              */
static size_t cnt  = 0;       /* number of terms stored               */
static seq_t  current_x = 0;  /* value of a(3k-2) for the next step   */

static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t newcap = cap ? cap * 2 : 8;
    while (newcap < needed) newcap <<= 1;           /* double until enough */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cap   = newcap;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 in universe of sequence */

    /* initialise with a(1)=3 if needed */
    if (cnt == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)3;
        cnt      = 1;
        current_x = (seq_t)3;
    }

    /* generate terms until we have at least n terms */
    while (cnt < (size_t)n) {
        seq_t x = current_x;
        seq_t y = (x * x - (seq_t)1) / (seq_t)2;   /* a(3k-1) */
        seq_t z = y + (seq_t)1;                    /* a(3k)   */
        seq_t w = x * y + (seq_t)1;                /* a(3k+1) */

        size_t need = cnt + 3;
        ensure_capacity(need);
        cache[cnt]     = y;
        cache[cnt + 1] = z;
        cache[cnt + 2] = w;
        cnt = need;
        current_x = w;   /* next first term of triple */
    }

    return cache[n - 1];
}
