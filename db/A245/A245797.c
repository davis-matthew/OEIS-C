/*
 * A245797  The number of labeled graphs of n vertices that have endpoints,
 *          where an endpoint is a vertex with degree 1.
 * https://oeis.org/A245797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245797: Number of labeled graphs on n vertices that have at least one endpoint
 * (vertex of degree 1).
 *
 * Let T(n) be the total number of labeled graphs (2^{n(n-1)/2}) and let
 * A059167(n) be the number of graphs with no degree‑1 vertices.
 * Then   a(n) = T(n) - A059167(n)
 *        = Σ_{k=1}^{n} (-1)^{k+1} C(n,k) *
 *          2^{(n-k)(n-k-1)/2} *
 *          Σ_{m=0}^{⌊k/2⌋} C(k,2m) * (2m-1)!! * (n-k)^{k-2m}.
 *
 * The inner sum comes from counting the ways in which a fixed set of k vertices
 * can all have degree 1; (2m-1)!! = (2m)!/(2^m m!) is the number of pairings
 * of 2m selected vertices.  The outer sum uses inclusion–exclusion over the
 * set of vertices that are endpoints.
 *
 * The implementation uses the helper functions
 *   binomial(n,k), pow2(exp), pow_uint(base,exp)
 *   (provided by the build environment) and works for
 *   arbitrary‑precision seq_t as well as for native integer types.
 * The routine is O(n²) in the worst case, but the constants are small
 * and it runs comfortably for n up to a few thousand.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* sequence starts at n = 1 */

    seq_t result = 0;

    /* allocate a reusable array for powers of (n-k) */
    static seq_t *pow_arr = NULL;
    static size_t pow_cap = 0;

    if (pow_cap < n + 1) {
        free(pow_arr);
        pow_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        pow_cap  = n + 1;
    }

    /* main inclusion–exclusion sum */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t c_nk = binomial(n, k);

        idx_t nk = n - k;
        idx_t exp = nk * (nk - 1) / 2;     /* exponent for 2^{...} */
        seq_t pow_e = pow2(exp);

        /* pre‑compute powers of (n-k) up to exponent k */
        pow_arr[0] = (seq_t)1;
        seq_t nk_seq = (seq_t)nk;
        for (idx_t t = 1; t <= k; ++t) {
            pow_arr[t] = pow_arr[t - 1] * nk_seq;
        }

        /* inner sum over m */
        seq_t S = 0;
        seq_t odd_fact = 1;      /* (2m-1)!! */
        idx_t max_m = k / 2;
        for (idx_t m = 0; m <= max_m; ++m) {
            if (m > 0) odd_fact *= (2 * m - 1);   /* update (2m-1)!! */

            seq_t c_k_2m   = binomial(k, 2 * m);
            seq_t pow_val  = pow_arr[k - 2 * m];
            seq_t term_m = c_k_2m;
            term_m *= odd_fact;
            term_m *= pow_val;
            S += term_m;
        }

        /* combine into the k‑th term of the outer sum */
        seq_t term_k = c_nk;
        term_k *= S;
        term_k *= pow_e;

        /* add or subtract based on (-1)^{k+1} */
        if ((k % 2) == 0) /* even k → negative sign */
            result -= term_k;
        else
            result += term_k;
    }
    return result;
}
