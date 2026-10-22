/*
 * A303579  Break up the list of values of the Euler totient function phi(k)
 *          into nondecreasing runs; sequence gives lengths of successive
 *          runs.
 * https://oeis.org/A303579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A303579: length of each non‑decreasing run in the totient sequence */
#include <stdlib.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    static idx_t current_k = 2;                    /* next φ index to evaluate   */
    static seq_t prev_phi = 1;                    /* φ(current_k-1)              */
    static idx_t current_run_len = 1;              /* length of the run being built */
    static idx_t runs_used = 0;                    /* number of finished runs stored*/
    static seq_t *runs = NULL;                    /* array of finished run lengths */
    static size_t runs_cap = 0;                    /* capacity of runs array      */

    /* Initialise on first call */
    if (!runs) {
        prev_phi = phi(1);                         /* φ(1) = 1                    */
        current_k = 2;                             /* start with k = 2             */
        current_run_len = 1;                       /* run starts with φ(1)         */
    }

    /* Produce runs until the nth one is finished and stored */
    while (runs_used < n) {
        seq_t phi_k = phi(current_k);

        if (phi_k < prev_phi) {                    /* run ends here               */
            if (runs_used >= runs_cap) {           /* grow array if needed        */
                size_t new_cap = runs_cap ? runs_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                if (!tmp) return 0;                /* allocation failure          */
                runs = tmp;
                runs_cap = new_cap;
            }
            runs[runs_used++] = current_run_len;   /* store finished run          */
            current_run_len = 1;                   /* start new run               */
        } else {
            ++current_run_len;                      /* continue current run        */
        }

        prev_phi = phi_k;
        ++current_k;
    }

    return runs[n - 1];
}
