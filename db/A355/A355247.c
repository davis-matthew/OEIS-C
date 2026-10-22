/*
 * A355247  Expansion of e.g.f. exp(2*(exp(x) - 1 + x)).
 * https://oeis.org/A355247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Bell(k+1) * Bell(n-k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------  Bell numbers cache -------------- */
static seq_t *bell = NULL;     /* array of Bell numbers B[0..]       */
static size_t bell_len = 0;    /* number of Bell numbers computed   */
static size_t bell_cap = 0;    /* current capacity of bell array    */

/* Ensure that Bell numbers up to B(upto) (inclusive) are computed. */
static void ensure_bell(size_t upto)
{
    if (bell_len > upto) return;      /* already computed */

    /* grow capacity to at least upto+1 (doubling strategy). */
    size_t new_cap = bell_cap ? bell_cap : 1;
    while (new_cap <= upto) new_cap <<= 1;

    if (!bell) {
        bell = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        bell = tmp;
    }
    bell_cap = new_cap;

    /* Compute Bell numbers from bell_len up to upto.  */
    if (bell_len == 0) {            /* initialise B(0) */
        bell[0] = (seq_t)1;
        bell_len = 1;
    }
    for (size_t m = bell_len; m <= upto; ++m) {
        seq_t value = 0;
        seq_t c = 1;                /* C(m-1,0) */
        size_t m1 = m - 1;
        for (size_t k = 0; k < m; ++k) {
            value += c * bell[k];
            /* next binomial coefficient: C(m-1,k+1) = C(m-1,k) * (m-1-k)/(k+1) */
            c = c * (m1 - k) / (k + 1);
        }
        bell[m] = value;
    }
    bell_len = upto + 1;
}

/* -------------------------------------------------- */
/*   Public sequence function                         */
/* -------------------------------------------------- */
seq_t a(idx_t n)
{
    size_t nn = (size_t)n;          /* index as ordinary integer */

    /* We need Bell numbers up to B(nn+1). */
    ensure_bell(nn + 1);

    seq_t result = 0;
    seq_t c = 1;                    /* C(nn,0) */

    for (size_t k = 0; k <= nn; ++k) {
        result += c * bell[k + 1] * bell[nn - k + 1];
        if (k < nn)                 /* prepare next binomial coefficient */
            c = c * (nn - k) / (k + 1);
    }
    return result;
}
