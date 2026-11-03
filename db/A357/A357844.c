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
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Build the smallest‑prime‑factor table up to limit. */
static void build_spf(idx_t limit, idx_t **spf_out)
{
    idx_t i, j;
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) {
        *spf_out = NULL;
        return;
    }

    for (i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;

    for (i = 2; i <= limit; ++i) {
        if (!spf[i]) {            /* i is prime */
            spf[i] = i;
            if (i <= limit / i)   /* avoid overflow in j loop */
                for (j = i * i; j <= limit; j += i)
                    if (!spf[j]) spf[j] = i;
        }
    }
    *spf_out = spf;
}

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
        den_arr     = mem_new(seq_t, sz);
        div_counts  = mem_new(seq_t, sz);
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
            den_arr    = mem_resize(seq_t, den_arr, newsize);
            div_counts = mem_resize(seq_t, div_counts, newsize);
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
