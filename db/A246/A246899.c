/*
 * A246899  Number of length 7+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined recursively by a(n) = 2*a(n-1) +7*a(n-2)
 *            -16*a(n-3) -20*a(n-4) +56*a(n-5) +28*a(n-6) -112*a(n-7)
 *            -14*a(n-8) +140*a(n-9) -14*a(n-10) -112*a(n-11) +28*a(n-12)
 *            +56*a(n-13) -20*a(n-14) -16*a(n-15) +7*a(n-16) +2*a(n-17)
 *            -a(n-18) with offset 1 (a(1) is the first term). The first 18
 *            term...
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Coefficients of the linear recurrence (a(n-1) .. a(n-18)) */
static const int coeff[18] = {
    2, 7, -16, -20, 56, 28, -112, -14, 140, -14,
   -112, 28, 56, -20, -16, 7, 2, -1
};

static seq_t *small_cache = NULL;   /* cache for a(1)..a(18) */
static idx_t  small_n    = 0;        /* number of small terms cached */

/* ------------------------------------------------------------ */
/* compute a(n) for n <= 18 by brute‑force DP on 11‑tuples      */
static seq_t compute_small(idx_t n)
{
    /* n is in [1,18] */
    size_t base = (size_t)(n + 1);
    size_t pow2 = base * base;          /* base^2   */
    size_t pow3 = pow2 * base;          /* base^3   */
    size_t pow4 = pow3 * base;          /* base^4   */

    seq_t *dp   = (seq_t*)malloc(pow4 * sizeof(seq_t));
    if (!dp) return 0;                  /* allocation failure -> 0 (never happens) */

    /* initial state: all 4‑tuples are possible, count = 1 */
    for (size_t i = 0; i < pow4; ++i) dp[i] = 1;

    /* length 5 .. 11 */
    for (size_t step = 5; step <= 11; ++step) {
        seq_t *next = (seq_t*)calloc(pow4, sizeof(seq_t));
        if (!next) { free(dp); return 0; }

        for (size_t idx = 0; idx < pow4; ++idx) {
            seq_t cnt = dp[idx];
            if (cnt == 0) continue;

            /* decode state (a,b,c,d) */
            unsigned d = (unsigned)(idx % base);
            unsigned c = (unsigned)((idx / base) % base);
            unsigned b = (unsigned)((idx / pow2) % base);
            unsigned a = (unsigned)((idx / pow3) % base);

            /* extend by new element e in [0, n] */
            for (unsigned e = 0; e < base; ++e) {
                /* check if any pair in (a,b,c,d,e) sums to n */
                if (a + b == n || a + c == n || a + d == n || a + e == n ||
                    b + c == n || b + d == n || b + e == n ||
                    c + d == n || c + e == n || d + e == n) {

                    size_t new_idx = ((b * base + c) * base + d) * base + e;
                    next[new_idx] += cnt;
                }
            }
        }
        free(dp);
        dp = next;
    }

    /* sum all counts for final length 11 */
    seq_t total = 0;
    for (size_t i = 0; i < pow4; ++i) total += dp[i];
    free(dp);
    return total;
}

/* ------------------------------------------------------------ */
/* main function requested by the specification                */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset */

    /*------------- compute/supply initial 18 terms -------------*/
    if (n <= 18) {
        if (!small_cache) {
            small_cache = (seq_t*)calloc(18, sizeof(seq_t));
        }
        if (small_n < n) {
            for (idx_t i = small_n; i < n; ++i) {
                small_cache[i] = compute_small((idx_t)(i + 1));
            }
            small_n = n;
        }
        return small_cache[n - 1];
    }

    /*------------- recursive part (n > 18) -------------*/
    /* ensure small terms are available */
    if (!small_cache) {
        small_cache = (seq_t*)calloc(18, sizeof(seq_t));
    }
    if (small_n < 18) {
        for (idx_t i = small_n; i < 18; ++i) {
            small_cache[i] = compute_small((idx_t)(i + 1));
        }
        small_n = 18;
    }

    /* cache for recurrence */
    static seq_t *rec_cache  = NULL;   /* holds a(1), a(2), ... */
    static idx_t  rec_len    = 0;     /* number of terms stored */
    static idx_t  rec_cap    = 0;     /* allocation capacity */

    if (rec_len < 18) {               /* initialise from small cache */
        rec_cap = (rec_len > 0) ? rec_len << 1 : 32;
        if (rec_cap < 18) rec_cap = 18;
        rec_cache = (seq_t*)realloc(rec_cache, rec_cap * sizeof(seq_t));
        for (unsigned i = 0; i < 18; ++i) rec_cache[i] = small_cache[i];
        rec_len = 18;
    }

    /* extend recurrence up to n */
    while (rec_len < n) {
        if (rec_len >= rec_cap) {      /* grow cache */
            rec_cap <<= 1;
            rec_cache = (seq_t*)realloc(rec_cache, rec_cap * sizeof(seq_t));
        }

        seq_t next = 0;
        for (int i = 0; i < 18; ++i) {
            seq_t term = 
                (seq_t)(coeff[i] >= 0 ? coeff[i] : -coeff[i]) *
                rec_cache[rec_len - 1 - i];
            if (coeff[i] >= 0) next += term;
            else               next -= term;
        }
        rec_cache[rec_len] = next;
        ++rec_len;
    }

    return rec_cache[n - 1];
}
