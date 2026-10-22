/*
 * A357738  Expansion of e.g.f. sin( 2 * (exp(x) - 1) )/2.
 * https://oeis.org/A357738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/2⌋} (-4)^k * Stirling2(n, 2*k+1)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper macros to satisfy both C and C++ compilers */
#ifdef __cplusplus
# define ALLOC_SEQ_T(sz) reinterpret_cast<seq_t*>(malloc(sz))
# define REALLOC_SEQ_T(ptr, sz) reinterpret_cast<seq_t*>(realloc(ptr, sz))
#else
# define ALLOC_SEQ_T(sz) ((seq_t*)malloc(sz))
# define REALLOC_SEQ_T(ptr, sz) ((seq_t*)realloc(ptr, sz))
#endif

seq_t a(idx_t n)
{
    /*  Persistent arrays for Stirling numbers (previous and current row)
        and the computed sequence values.
        All allocations are done once and then grows as needed. */
    static seq_t *S_prev = NULL;     /* S(n-1, *) */
    static seq_t *S_cur  = NULL;     /* S(n,   *) */
    static seq_t *A_seq  = NULL;     /* a(n)   */
    static idx_t  prev_size = 0;     /* allocated size of all arrays */
    static idx_t  computed  = 0;     /* highest n for which a(n) was computed */

    /* Initial allocation for n = 0 */
    if (prev_size == 0) {
        prev_size = 1;
        S_prev = ALLOC_SEQ_T(prev_size * sizeof(seq_t));
        S_cur  = ALLOC_SEQ_T(prev_size * sizeof(seq_t));
        A_seq  = ALLOC_SEQ_T(prev_size * sizeof(seq_t));
        if (!S_prev || !S_cur || !A_seq) abort();  /* memory error */

        S_prev[0] = (seq_t)1;  /* Stirling(0,0) = 1   */
        S_cur[0]  = (seq_t)0;  /* unused for n=0      */
        A_seq[0]  = (seq_t)0;  /* a(0) = 0            */
        computed  = 0;
    }

    /* Grow the arrays if we need a larger n */
    if (n > computed) {
        idx_t new_size = n + 1;
        if (new_size > prev_size) {
            seq_t *tmp;

            tmp = REALLOC_SEQ_T(S_prev, new_size * sizeof(seq_t));
            if (!tmp) abort(); S_prev = tmp;

            tmp = REALLOC_SEQ_T(S_cur,  new_size * sizeof(seq_t));
            if (!tmp) abort(); S_cur = tmp;

            tmp = REALLOC_SEQ_T(A_seq,  new_size * sizeof(seq_t));
            if (!tmp) abort(); A_seq = tmp;

            /* Zero the newly allocated entries so that
               references to S_prev[k] or S_cur[k] that are
               out of current row bounds yield 0.          */
            for (idx_t i = prev_size; i < new_size; ++i) {
                S_prev[i] = (seq_t)0;
                S_cur[i]  = (seq_t)0;
                A_seq[i]  = (seq_t)0;
            }
            prev_size = new_size;
        }

        /* Compute all missing rows up to n */
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* Stirling number S(i,0) = 0 for i >= 1 */
            S_cur[0] = (seq_t)0;

            /* Compute S(i,k) for k = 1..i using recurrence
               S(i,k) = k*S(i-1,k) + S(i-1,k-1)
               and the fact that S_prev holds S(i-1,*).        */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (k < prev_size) ? S_prev[k] : (seq_t)0;
                seq_t term2 = (k-1 < prev_size) ? S_prev[k-1] : (seq_t)0;
                S_cur[k] = term1 * (seq_t)k + term2;
            }

            /* Compute a(i) from the odd Stirling numbers of this row.
               The odd indices are 1,3,5,... up to i (inclusive if odd).   */
            seq_t a_val = (seq_t)0;
            seq_t pow_factor = (seq_t)1;   /* (-4)^0 */
            for (idx_t m = 1; m <= i; m += 2) {
                a_val += pow_factor * S_cur[m];
                pow_factor *= (seq_t)(-4);
            }
            A_seq[i] = a_val;

            /* Prepare for next iteration: swap S_prev and S_cur */
            seq_t *tmp = S_prev;
            S_prev = S_cur;
            S_cur = tmp;
        }

        computed = n;
    }

    return A_seq[n];
}
