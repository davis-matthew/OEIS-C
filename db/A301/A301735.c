/*
 * A301735  Coefficients of q-series associated with the colored tetrahedron
 *          graph J_{6j,0}. Also the q-series J0(q,1) of Flajolet and
 *          Sedgewick.
 * https://oeis.org/A301735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = [q^n] Sum_{k>=0} (-1)^k * q^{k*(k+1)/2} / (q; k)^2
 *   where (q; k) = Prod_{j=1..k} (1 - q^j)
 *
 * The implementation uses the recurrence
 *   P_k(q) = Product_{j=1..k} (1 - q^j)^{-2}
 *   P_k(m) - 2 P_k(m-k) + P_k(m-2k) = P_{k-1}(m)
 * to compute the coefficients of P_k efficiently.
 * Then
 *   a(n) = Sum_{k>=0} (-1)^k * P_k(n - k*(k+1)/2),   for n>=0
 *
 * The sequence is memoised: the first call calculates all terms up to the
 * requested index, and further calls return cached values.  The algorithm
 * runs in O(N √N) time and uses O(N) memory, where N is the
 * requested index.
 */

#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;   /* cached values of a(0..M) */
static idx_t cache_size = 0;  /* number of cached elements (includes index 0) */

/* Compute all values a(0)..a(N) and store them in cache */
static void compute_up_to(idx_t N)
{
    /* Allocate new cache */
    seq_t *new_cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!new_cache) return;  /* allocation failed; abort silently */

    /* Initial term a(0) = 1 */
    new_cache[0] = 1;

    /* Determine largest k with k*(k+1)/2 <= N */
    idx_t max_k = 0;
    for (;; ++max_k) {
        idx_t next = max_k + 1;
        if (next * (next + 1) / 2 > N) {
            max_k = next - 1;
            break;
        }
    }

    /* p_prev holds coefficients of P_{k-1} */
    seq_t *p_prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!p_prev) return;
    p_prev[0] = 1;  /* P_0(q) = 1 */

    /* Iterate over outer sum index k = 1..max_k */
    for (idx_t k = 1; k <= max_k; ++k) {
        /* Allocate array for P_k coefficients */
        seq_t *p_cur = (seq_t *)calloc(N + 1, sizeof(seq_t));
        if (!p_cur) {
            free(p_prev);
            free(new_cache);
            return;
        }

        idx_t offset = k * (k + 1) / 2;
        seq_t sign = (k & 1) ? -1 : 1;

        /* Compute coefficients of P_k using the recurrence */
        for (idx_t m = 0; m <= N; ++m) {
            seq_t val = p_prev[m];
            if (m >= k)      val += 2 * p_cur[m - k];
            if (m >= 2 * k)  val -= p_cur[m - 2 * k];
            p_cur[m] = val;
        }

        /* Add contribution of this k to a(m) for all m >= offset */
        if (offset <= N) {
            for (idx_t m = offset; m <= N; ++m) {
                new_cache[m] += sign * p_cur[m - offset];
            }
        }

        /* Prepare for next iteration */
        free(p_prev);
        p_prev = p_cur;
    }

    /* Free remaining array */
    free(p_prev);

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_size = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        compute_up_to(n);
    }
    return cache[n];
}
