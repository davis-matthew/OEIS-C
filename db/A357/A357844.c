/*
 * A357844  Denominators of the partial alternating sums of the reciprocals of
 *          the number of divisors function (A000005).
 * https://oeis.org/A357844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the partial alternating sum S(n) =
 *            Σ_{k=1..n} (-1)^{k+1} / d(k), where d(k) is the divisor count
 *            (OEIS A000005)
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   build_spf, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include "../utils/gcd.h"
#include "../utils/build_spf.h"

static idx_t max_n = 0;          /* highest n processed so far       */
static seq_t *den_arr = NULL;    /* den_arr[i] = denominator of S(i) */
static seq_t *div_counts = NULL; /* divisor counts up to max_n        */
static idx_t *spf = NULL;        /* smallest prime factor table       */
static idx_t spf_len = 0;        /* current size of spf array         */

static seq_t curr_num_abs = 0;   /* absolute value of current numerator  */
static seq_t curr_den     = 1;   /* current denominator                  */
static int   curr_sign     = 1;   /* +1 or -1 (ignored when numerator is 0) */

seq_t a(idx_t n)
{
    if (n == 0) return 1; /* not used, but keep defined */

    /* ---- initialise structures on first call ---- */
    if (max_n == 0) {
        size_t sz = (size_t)(n + 1);
        den_arr     = (seq_t *)malloc(sz * sizeof(seq_t));
        div_counts  = (seq_t *)malloc(sz * sizeof(seq_t));
        assert(den_arr && div_counts);

        /* build smallest prime factor table up to n */
        build_spf(n, &spf);
        spf_len = n + 1;

        /* initialise d(1) */
        div_counts[1] = 1;
        den_arr[0] = 1;    /* dummy value for index 0 */
        /* curr sum already 0/1 */
    }

    /* ---- extend to requested n if necessary ---- */
    if (n > max_n) {
        size_t newsize = (size_t)(n + 1);
        if (max_n != 0) {
            den_arr    = (seq_t *)realloc(den_arr,  newsize * sizeof(seq_t));
            div_counts = (seq_t *)realloc(div_counts, newsize * sizeof(seq_t));
            assert(den_arr && div_counts);
        }

        if (spf_len <= n) {
            free(spf);
            build_spf(n, &spf);
            spf_len = n + 1;
        }

        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* compute d(k) */
            if (k == 1) {
                div_counts[1] = 1;
            } else {
                idx_t p = spf[k];
                idx_t m = k / p;
                int   exp = 1;
                while (m % p == 0) {
                    m /= p;
                    exp++;
                }
                div_counts[k] = div_counts[m] * (seq_t)(exp + 1);
            }

            /* add the term (-1)^{k+1} / d(k) to the running sum */
            seq_t dk = div_counts[k];
            int   term_sign = (k & 1) ? +1 : -1;          /* odd -> +, even -> - */

            seq_t a_val = curr_num_abs * dk;             /* |curr_num| * dk  */
            seq_t b_val = curr_den;                      /* current denominator */

            seq_t new_num_abs;
            int   new_sign;

            if (curr_num_abs == 0) {
                new_num_abs = 1;
                new_sign    = term_sign;
            } else if (curr_sign == term_sign) {
                new_num_abs = a_val + b_val;
                new_sign    = curr_sign;
            } else {
                if (a_val >= b_val) {
                    new_num_abs = a_val - b_val;
                    new_sign    = curr_sign;
                } else {
                    new_num_abs = b_val - a_val;
                    new_sign    = term_sign;
                }
            }

            seq_t new_den = curr_den * dk;
            seq_t g = gcd(new_num_abs, new_den);
            new_num_abs /= g;
            new_den     /= g;

            curr_num_abs = new_num_abs;
            curr_den     = new_den;
            curr_sign    = (curr_num_abs == 0) ? 1 : new_sign;

            den_arr[k] = curr_den;
        }
        max_n = n;
    }

    return den_arr[n];
}
