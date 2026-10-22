/*
 * A382847  a(n) = Sum_{k=0..n} binomial(n+k-1,k) * (Stirling2(n,k) * k!)^2.
 * https://oeis.org/A382847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k-1,k) * (Stirling2(n,k) * k!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *S      = NULL;   /* array of computed a(m) */
    static idx_t S_cap   = 0;      /* capacity of S */
    static idx_t S_len   = 0;      /* number of terms computed so far */

    /* Cache for the current row of T(n,k) = Stirling2(n,k) * k!  */
    static seq_t *Tprev  = NULL;   /* array of length (current_n + 1) */
    static idx_t  T_len  = 0;      /* current length of Tprev */

    /* Helper to compute terms up to target */
    if (n >= S_len) {
        /* Ensure S has capacity up to n */
        if (n + 1 > S_cap) {
            seq_t *tmp = (seq_t *)realloc(S, sizeof(seq_t) * (n + 1));
            if (!tmp) { /* allocation failure: abort */
                exit(EXIT_FAILURE);
            }
            S = tmp;
            S_cap = n + 1;
        }

        /* Initialization for n = 0 */
        if (S_len == 0) {
            S[0] = 1;
            S_len = 1;

            Tprev = (seq_t *)malloc(sizeof(seq_t) * 1);
            if (!Tprev) exit(EXIT_FAILURE);
            Tprev[0] = 1;   /* T(0,0) = 1 */
            T_len  = 1;
        }

        /* Compute terms from S_len to n */
        for (idx_t cur_n = S_len; cur_n <= n; ++cur_n) {
            /* Allocate next row of T */
            seq_t *Tnext = (seq_t *)malloc(sizeof(seq_t) * (cur_n + 1));
            if (!Tnext) exit(EXIT_FAILURE);
            Tnext[0] = 0;   /* T(cur_n,0) = 0 for cur_n > 0 */

            /* Compute T(cur_n,k) for k = 1 .. cur_n */
            for (idx_t k = 1; k <= cur_n; ++k) {
                seq_t a_val = 0;
                if (k <  T_len) a_val += Tprev[k];
                if (k-1 < T_len) a_val += Tprev[k-1];
                Tnext[k] = (seq_t)k * a_val;
            }

            /* Compute a(cur_n) = sum_{k} B(n,k) * Tnext[k]^2 */
            seq_t sum = 0;
            seq_t B   = 1;   /* B(cur_n,0) = 1 */

            for (idx_t k = 0; k <= cur_n; ++k) {
                seq_t Tval = Tnext[k];
                seq_t T2   = Tval * Tval;
                sum += B * T2;
                if (k < cur_n) {
                    /* B(n,k+1) = B(n,k) * (n + k) / (k + 1) */
                    B = B * (seq_t)(cur_n + k) / (seq_t)(k + 1);
                }
            }

            /* Store the computed term */
            S[cur_n] = sum;
            ++S_len;

            /* Free the previous row and update pointers */
            free(Tprev);
            Tprev = Tnext;
            T_len = cur_n + 1;
        }
    }

    return S[n];
}
