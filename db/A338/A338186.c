/*
 * A338186  Expansion of g.f. 2*(1-3*x-6*x^2)/((1-x)^2*(1-9*x)).
 * https://oeis.org/A338186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(2*n+1) + 4*n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of powers of 9: pow9[i] = 9^i */
static seq_t *pow9 = NULL;
static size_t pow9_cap = 0;
static size_t pow9_len = 0;

/* Grow the cache so that pow9[needed] exists */
static void grow_pow9(size_t needed)
{
    /* if already sufficient, nothing to do */
    if (pow9_len > needed)
        return;

    /* determine new capacity */
    size_t new_cap = pow9_cap ? pow9_cap * 2 : 1;
    while (new_cap <= needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(pow9, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();        /* allocation failed – let the process terminate */

    pow9 = tmp;
    pow9_cap = new_cap;

    /* initialise the first element if this is the first allocation */
    if (pow9_len == 0) {
        pow9[0] = 1;
        pow9_len = 1;
    }

    /* extend the cache up to 'needed' */
    while (pow9_len <= needed) {
        pow9[pow9_len] = pow9[pow9_len - 1] * ((seq_t)9);
        pow9_len++;
    }
}

/* OEIS sequence A338186 */
seq_t a(idx_t n)
{
    /* use powers of 9 to avoid large intermediate multiplication */
    size_t need = (size_t)n;
    grow_pow9(need);

    seq_t X = pow9[need];                       /* X = 9^n (odd) */
    seq_t k = (X - (seq_t)1) / ((seq_t)2);      /* k = (X-1)/2 */
    seq_t term1 = k * ((seq_t)3);               /* 3*k  */
    seq_t term2 = ((seq_t)2) * ((seq_t)n + (seq_t)1); /* 2*(n+1) */
    return term1 + term2;
}
