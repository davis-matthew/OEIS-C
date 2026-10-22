/*
 * A357830  a(n) = Sum_{k=0..floor((n-2)/3)} |Stirling1(n,3*k+2)|.
 * https://oeis.org/A357830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/3)} |Stirling1(n,3*k+2)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is computed via a linear recurrence on the coefficients of the
 * rising factorial modulo x^3-1:
 *   coeff[0], coeff[1], coeff[2] evolve as
 *   tmp0 = coeff0*(n-1) + coeff2;
 *   tmp1 = coeff0 + coeff1*(n-1);
 *   tmp2 = coeff1 + coeff2*(n-1);
 *   a(n) = coeff2.
 * The routine memoises computed values so that multiple calls
 * in arbitrary order are fast.
 */
#include <stdlib.h>

static seq_t coeff0 = 0;
static seq_t coeff1 = 0;
static seq_t coeff2 = 0;
static idx_t max_comp = 0;
static size_t capacity = 0;
static seq_t *a_cache = NULL;

static void init_seq(void) {
    coeff0 = 1;          /* coeff at n = 0 */
    coeff1 = 0;
    coeff2 = 0;
    max_comp = 0;
    capacity = 1;
    a_cache = (seq_t *)malloc(sizeof(seq_t));
    a_cache[0] = coeff2; /* a(0) = 0 */
}

seq_t a(idx_t n) {
    if (!a_cache) {          /* first call – initialise */
        init_seq();
    }
    if (n > max_comp) {      /* need further values */
        /* grow cache if necessary */
        if ((size_t)n + 1 > capacity) {
            size_t new_cap = capacity ? capacity : 1;
            while (new_cap <= (size_t)n) {
                new_cap <<= 1;
            }
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            capacity = new_cap;
        }
        /* compute up to n, updating coefficients */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t term = (seq_t)(i - 1);              /* i-1 */
            seq_t tmp0 = coeff0 * term + coeff2;
            seq_t tmp1 = coeff0 + coeff1 * term;
            seq_t tmp2 = coeff1 + coeff2 * term;
            coeff0 = tmp0;
            coeff1 = tmp1;
            coeff2 = tmp2;
            a_cache[i] = coeff2;                     /* store a(i) */
        }
        max_comp = n;
    }
    return a_cache[n];
}
