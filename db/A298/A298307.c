/*
 * A298307  Start with a(0) = 1 and add at step n >= 0 the term 1 at position
 *          floor(4*(n+a(n))/3).
 * https://oeis.org/A298307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A298307: iterative simulation of sequence b starting with b[0]=1;
   for n ≥ 0 increment b[floor(4*(n+b[n])/3)]++.  */
#include <stdlib.h>

/* static state */
static seq_t *b = NULL;        /* storage for b[n] */
static size_t cap = 0;         /* current capacity of b */
static idx_t next_step = 0;    /* smallest step not yet processed */

/* ensure the array can hold index 'idx' */
static void ensure_capacity(idx_t idx)
{
    if (idx < cap) return;
    size_t new_cap = (cap == 0) ? 1 : cap;
    while (idx >= new_cap) new_cap <<= 1;
    seq_t *newb;
#ifdef __cplusplus
    /* allocate new array with constructor for class types */
    newb = new seq_t[new_cap];
    /* copy old data */
    if (b != NULL) {
        for (size_t i = 0; i < cap; ++i) newb[i] = b[i];
        delete[] b;
    }
    /* zero new elements */
    for (size_t i = cap; i < new_cap; ++i) newb[i] = 0;
#else
    newb = (seq_t*)realloc(b, new_cap * sizeof(seq_t));
    if (!newb) abort();                 /* allocation failure */
    /* zero new elements */
    for (size_t i = cap; i < new_cap; ++i) newb[i] = 0;
#endif
    b = newb;
    cap = new_cap;
}

/* simulate one step numbered k */
static void simulate_step(idx_t k)
{
    ensure_capacity(k);
    seq_t cur = b[k];
    /* pos = floor(4*(k + cur) / 3)  */
    idx_t pos = (idx_t)(((seq_t)4 * ((seq_t)k + cur)) / 3);
    ensure_capacity(pos);
    b[pos] = b[pos] + 1;
}

/* public function */
seq_t a(idx_t n)
{
    /* lazily initialize */
    if (b == NULL) {
        ensure_capacity(0);     /* at least size 1 */
        b[0] = 1;
        next_step = 0;
    }
    /* process all pending steps up to n */
    while (next_step <= n) {
        simulate_step(next_step);
        next_step++;
    }
    return b[n];
}
