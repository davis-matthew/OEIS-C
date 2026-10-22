/*
 * A332842  Initial digit of n-th superfactorial.
 * https://oeis.org/A332842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = first decimal digit of the n-th superfactorial
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) = A000030(A000178(n))) */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Caches of the cumulative logarithms  (base 10) of factorials
       and superfactorials.  They are expanded only as far as needed. */
    static long double *log_fact  = NULL; /* log10(k!)   for 0 <= k <= max_n */
    static long double *log_super = NULL; /* log10(sf(k)) for 0 <= k <= max_n */
    static idx_t max_n = 0;              /* highest index cached so far */

    /* ------------------------------------------------------------- */
    /* Initialisation: first call creates the size‑1 tables.         */
    /* ------------------------------------------------------------- */
    if (log_fact == NULL) {
        size_t sz = 1;                     /* need entry for k = 0          */
        log_fact  = (long double *)calloc(sz, sizeof(long double));
        log_super = (long double *)calloc(sz, sizeof(long double));
        if (log_fact == NULL || log_super == NULL) {
            /* Allocation failure – return an arbitrary value */
            free(log_fact);  free(log_super);
            log_fact = log_super = NULL;
            return (seq_t)0;
        }
        log_fact[0]  = 0.0L;  /* log10(0!)   = 0 */
        log_super[0] = 0.0L;  /* log10(sf(0)) = 0 (empty product) */
        max_n = 0;
    }

    /* ------------------------------------------------------------- */
    /* Extend the caches up to the requested index n, if necessary. */
    /* ------------------------------------------------------------- */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);
        long double *tmp_fact  = (long double *)realloc(log_fact,  new_size * sizeof(long double));
        long double *tmp_super = (long double *)realloc(log_super, new_size * sizeof(long double));
        if (tmp_fact == NULL || tmp_super == NULL) {
            /* In case of allocation failure, preserve the old tables. */
            free(tmp_fact);  free(tmp_super);
            return (seq_t)0;
        }
        log_fact  = tmp_fact;
        log_super = tmp_super;

        /* Compute the new entries incrementally. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            log_fact[i]  = log_fact[i - 1] + log10l((long double)i);
            log_super[i] = log_super[i - 1] + log_fact[i];
        }
        max_n = n;
    }

    /* ------------------------------------------------------------- */
    /* Determine the leading decimal digit from the fractional part. */
    /* ------------------------------------------------------------- */
    long double total_log = log_super[n];
    long double frac_part = total_log - floorl(total_log);   /* 0 <= frac < 1 */
    long double pow10_frac = powl(10.0L, frac_part);

    int digit = (int)(pow10_frac + 1e-12L);  /* tiny epsilon to avoid 10.0 */
    if (digit == 0) digit = 1;               /* safeguard for rounding errors */

    return (seq_t)digit;
}
