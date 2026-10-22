/*
 * A369853  a(n) = number of subsets of {1,2,...,n} that contain more
 *          nonprimes than primes.
 * https://oeis.org/A369853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} containing more non‑primes
 *            than primes. Computation: a(n) = 2^n – Σ_{p=0}^{P(n)} C(P(n),p)
 *            * Σ_{c=0}^{min(p,NP(n))} C(NP(n),c)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq_array, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Count primes ≤ n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    idx_t P = (idx_t)prime_cnt;          /* number of primes */
    idx_t NP = n - P;                    /* number of non‑primes */

    free(primes);                        /* free allocated prime list */

    /* Allocate arrays for the binomial coefficients of NP. */
    size_t sz = (size_t)NP + 1;          /* NP ≤ n */
    seq_t *C   = alloc_seq_array(sz);
    seq_t *pref = alloc_seq_array(sz);

    if (!C || !pref) {                  /* allocation failure */
        free_seq_array(C);
        free_seq_array(pref);
        return (seq_t)0;
    }

    /* Compute C(NP,i) recursively and store prefix sums. */
    seq_t comb = 1;
    for (idx_t i = 0; i <= NP; ++i) {
        C[i] = comb;
        if (i < NP) {
            comb = comb * (seq_t)(NP - i) / (seq_t)(i + 1);
        }
    }

    seq_t sum = 0;
    for (idx_t i = 0; i <= NP; ++i) {
        sum += C[i];
        pref[i] = sum;
    }

    /* Sum over all possible prime selections. */
    seq_t S = 0;
    seq_t binP = 1;                       /* C(P,0) */
    for (idx_t p = 0; p <= P; ++p) {
        idx_t lim = (p <= (idx_t)NP) ? p : (idx_t)NP;
        S += binP * pref[lim];
        if (p < P) {
            binP = binP * (seq_t)(P - p) / (seq_t)(p + 1);
        }
    }

    seq_t total = pow2(n);                 /* 2^n in seq_t */
    seq_t result = total - S;              /* subsets with more non‑primes */

    /* Clean up. */
    free_seq_array(C);
    free_seq_array(pref);

    return result;
}
