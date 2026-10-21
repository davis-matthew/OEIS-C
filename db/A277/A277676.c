/*
 * A277676  Numbers k such that d(k+2) > d(k+1), where d(m) is the number of
 *          digits in the base-m representation of m!.
 * https://oeis.org/A277676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = k where d(k+2) > d(k+1); number of digits in base m of m!
 *            is computed via logarithms. The sequence is generated lazily and
 *            stored in a static cache so that any call order works
 * Range:     u8 n <= 210, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for sequence values */
    static seq_t *seq   = NULL;
    static idx_t seq_cap = 0;      /* number of entries allocated */
    static idx_t seq_len = 0;      /* number of entries stored */

    /* static cache for logarithms of factorials */
    static long double *log_fact = NULL;  /* log10(m!) for m = 0 .. max_m */
    static size_t log_cap  = 0;    /* allocated size of log_fact array */

    /* variables needed while generating up to the nth element */
    static idx_t computed_m = 1;   /* current m processed (m! computed) */
    static long double prev_log_fact = 0.0L;  /* log10((m-1)!) when m is processed */
    static int prev_digits = 0;    /* D(m-1) = digits in base (m-1) of (m-1)! */

    /* epsilon for rounding when using floor; tiny guard against FP error */
    const long double EPS = 1e-12L;

    /* initialize caches on first call */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));

        log_cap = 32;
        log_fact = (long double *)calloc(log_cap, sizeof(long double));
        /* log_fact[0] = log10(0!) = 0 */
        log_fact[0] = 0.0L;
        /* log_fact[1] = log10(1!) = 0 */
        if (log_cap > 1) log_fact[1] = 0.0L;

        /* start at m = 2 to have a meaningful prev_digits */
        computed_m = 2;
        log_fact[2] = log_fact[1] + log10l(2.0L);
        prev_digits = (int)(floor(log_fact[2]/log10l(2.0L) + EPS) + 1);
        prev_log_fact = log_fact[2];
    }

    /* produce sequence entries until we have at least n of them */
    while (seq_len < n) {
        /* advance to next m */
        computed_m++;

        /* grow log_fact array if needed */
        if ((size_t)computed_m >= log_cap) {
            size_t new_cap = log_cap * 2;
            while (new_cap <= (size_t)computed_m) new_cap <<= 1;
            log_fact = (long double *)realloc(log_fact, new_cap * sizeof(long double));
            log_cap = new_cap;
        }

        /* compute log10(computed_m!) */
        log_fact[computed_m] = log_fact[computed_m - 1] + log10l((long double)computed_m);

        /* number of digits of computed_m! in base computed_m */
        int digits = (int)(floor(log_fact[computed_m] / log10l((long double)computed_m) + EPS) + 1);

        /* check if digit count increased compared to previous m-1 */
        if (digits > prev_digits) {
            /* store k = m-2 in the sequence array */
            if (seq_len + 1 > seq_cap) {
                size_t new_cap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)(computed_m - 2);
        }

        /* update previous value for next iteration */
        prev_digits = digits;
    }

    return seq[n - 1];   /* indices are 1‑based in the specification */
}
