/*
 * A251684  G.f.: exp( Sum_{n>=1} A047863(n)*x^n/n ), where A047863(n) =
 *          Sum_{k=0..n} binomial(n, k) * (2^k)^(n-k).
 * https://oeis.org/A251684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 1;
 *           for n ≥ 1, a(n) = (1/n) * Σ_{k=1..n} a(n-k) * Ck,
 * where  Ck = Σ_{j=0..k} binom(k,j) * 2^{ j*(k-j) }  (A047863(k)).
 *
 * The routine keeps a static cache so that calls with arbitrary indices are answered
 * in O(n²) time and O(n) memory for the requested maximum n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for values up to the maximum n requested so far */
    static idx_t  max_n = 0;
    static seq_t *a_arr = NULL;   /* a values    */
    static seq_t *c_arr = NULL;   /* Ck values   */

    if (n == 0) {
        /* Ensure the base case is initialized before returning */
        if (a_arr == NULL) {
            a_arr = (seq_t *)malloc(sizeof(seq_t));
            a_arr[0] = (seq_t)1;
            c_arr = (seq_t *)malloc(sizeof(seq_t));
            c_arr[0] = (seq_t)1;
            max_n = 0;
        }
        return a_arr[0];
    }

    /* Extend the cache if necessary */
    if (n > max_n) {
        size_t new_len = (size_t)n + 1;

        /* Grow a_arr */
        seq_t *tmp = (seq_t *)realloc(a_arr, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        a_arr = tmp;

        /* Grow c_arr */
        tmp = (seq_t *)realloc(c_arr, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        c_arr = tmp;

        /* Initialize base case if this is the first allocation */
        if (max_n == 0) {
            a_arr[0] = (seq_t)1;
            c_arr[0] = (seq_t)1;
        }

        /* Compute new entries */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* ---- compute Ck (the A047863 value) ---- */
            seq_t bin      = (seq_t)1;   /* binom(k,0)      */
            seq_t pow_term = (seq_t)1;   /* = 2^{0}         */
            seq_t sum_c    = (seq_t)0;

            for (idx_t j = 0; j <= k; ++j) {
                /* Term contribution: binom(k,j) * 2^{j*(k-j)} */
                sum_c += bin * pow_term;

                if (j < k) {
                    /* Update binom(k,j) → binom(k,j+1)  */
                    bin = (bin * (seq_t)(k - j)) / (seq_t)(j + 1);

                    /* Difference between successive exponents:
                     * d = (j+1)*(k-(j+1)) - j*(k-j) = k - 2*j - 1
                     * Multiply/divide pow_term by 2^d accordingly.  */
                    int d = (int)((int)k - 2 * (int)j - 1);
                    if (d > 0)
                        pow_term <<= (unsigned)d;
                    else if (d < 0)
                        pow_term >>= (unsigned)(-d);
                }
            }
            c_arr[k] = sum_c;

            /* ---- compute a_k using the recurrence ---- */
            seq_t sum_a = (seq_t)0;
            for (idx_t t = 1; t <= k; ++t) {
                sum_a += c_arr[t] * a_arr[k - t];
            }
            a_arr[k] = sum_a / (seq_t)k;
        }
        max_n = n;
    }

    return a_arr[n];
}
