/*
 * A329130  a(0)=0; for any n >= 0, if a(n) > n then a(n+1) = a(n) - n,
 *          otherwise a(n+1) = a(n) + k, where k is the total number of terms
 *          a(m) <= m with m <= n.
 * https://oeis.org/A329130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  look, easy, nonn
 * Formula:   a(0)=0; for n>=0, if a(n)>n then a(n+1)=a(n)-n, else
 *            a(n+1)=a(n)+k, where k = |{ m | 0 <= m <= n, a(m)<=m }|
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq      = NULL;   /* array of computed terms */
static idx_t   capacity = 0;      /* allocated array capacity      */
static idx_t   seq_size = 0;      /* number of terms stored       */
static idx_t   count_k  = 0;      /* number of indices <= current with a(m)<=m */

/* returns a(n) for any n >= 0, memoizing values for later use      */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!seq) {
        capacity = 1;
        seq      = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq) abort();
        seq[0]    = (seq_t)0;   /* a(0) = 0 */
        seq_size  = 1;
        count_k   = 1;          /* a(0)<=0 is counted */
    }

    /* extend the memoised array up to n if required */
    if (n >= seq_size) {
        idx_t new_size = n + 1;
        if (new_size > capacity) {
            idx_t new_cap = new_size * 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!seq) abort();
            capacity = new_cap;
        }
        for (idx_t i = seq_size; i < new_size; ++i) {
            seq_t prev = seq[i - 1];
            seq_t next;
            if (prev > (seq_t)(i - 1)) {          /* a(i-1) > i-1 */
                next = prev - (seq_t)(i - 1);
            } else {                               /* a(i-1) <= i-1 */
                next = prev + (seq_t)count_k;
            }
            seq[i] = next;
            if (next <= (seq_t)i) {
                ++count_k;
            }
        }
        seq_size = new_size;
    }

    return seq[n];
}
