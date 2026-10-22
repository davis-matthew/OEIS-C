/*
 * A334525  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2 + 7xy = 0.
 * https://oeis.org/A334525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1
 * Range:     u8 n <= 41, u16 n <= 5340, u32 n <= 9498, u64 n <= 9498,
 *            u128 n <= 9498, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          + sum_{k=1}^{⌊n/7⌋} 4 *                             */
/*              #{ d | d divides 7*k^2,  d ≤ n, (7*k^2)/d ≤ n }  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Static SPF array for numbers up to the current limit */
static idx_t *spf_arr = NULL;
static idx_t spf_limit = 0;

/* Ensure SPF array is up to at least @limit */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf_arr) free(spf_arr);
    build_spf(limit, &spf_arr);
    spf_limit = limit;
}

/* Recursive divisor enumeration with bounds check */
static void count_divs(const idx_t *pr, const unsigned int *exp, size_t pf_cnt,
                       size_t pos, __int128 cur, __int128 M,
                       __int128 n_val, size_t *res)
{
    if (pos == pf_cnt) {
        if (cur <= n_val && M / cur <= n_val) ++(*res);
        return;
    }
    __int128 val = cur;
    for (unsigned int e = 0; e <= exp[pos]; ++e) {
        count_divs(pr, exp, pf_cnt, pos + 1, val, M, n_val, res);
        val *= pr[pos];
    }
}

/* ---------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Base case: w = 0 */
    seq_t result = (seq_t)n * 4 + 1;

    /* If n < 7, no non‑zero w solutions exist */
    if (n < 7) return result;

    idx_t kmax = n / 7;
    ensure_spf(kmax);

    /* Temporary storage for factorisation */
    idx_t pp[64];
    unsigned int ee[64];
    const size_t PP_MAX = 64;

    for (idx_t k = 1; k <= kmax; ++k) {
        /* Factorize k using SPF */
        idx_t tmp = k;
        size_t pf_cnt = 0;
        bool has7 = false;

        while (tmp > 1) {
            idx_t p = spf_arr[tmp];
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            pp[pf_cnt] = p;
            if (p == 7) {
                ee[pf_cnt] = 2 * e + 1; /* exponent of 7 in 7*k^2 */
                has7 = true;
            } else {
                ee[pf_cnt] = 2 * e;
            }
            ++pf_cnt;
        }

        if (!has7) {
            /* 7 not in k, must add it once */
            pp[pf_cnt] = 7;
            ee[pf_cnt] = 1;            /* exponent of 7 in 7*k^2 */
            ++pf_cnt;
        }

        /* Count admissible divisors of 7*k^2 */
        __int128 M = (__int128)7 * k * k;
        __int128 n_val = (__int128)n;
        size_t subcnt = 0;
        count_divs(pp, ee, pf_cnt, 0, (__int128)1, M, n_val, &subcnt);

        /* Each admissible divisor gives 4 ordered (x,y) pairs
           multiplied by the two possible signs of w. */
        result += (seq_t)(4 * subcnt);
    }

    return result;
}
