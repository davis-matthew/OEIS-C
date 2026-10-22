/*
 * A368010  Ordinal transform of the leading digit of the factorial numbers.
 * https://oeis.org/A368010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = |{ j in {0..n} : leading digit of j! = leading digit of
 *            n! }|
 * Range:     u8 n <= 845, u16 n <= 9688, u32 n <= 9688, u64 n <= 9688,
 *            u128 n <= 9688, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental computation of leading digits of factorials
   via double-precision logarithms.  A static array caches the leading digit of
   k! for 0 ≤ k ≤ max_computed.  A cumulative count array stores how many
   factorials up to the current index have each leading digit 1..9, so that
   a(n) is obtained in O(1) after ensuring the cache covers n. */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static seq_t *lead_cache = NULL;    /*  leading digits of k! */
static size_t lead_cap   = 0;       /*  capacity of lead_cache      */
static size_t lead_len   = 0;       /*  number of filled entries    */
static double log_sum    = 0.0;     /*  cumulative log10 sum         */
static uint32_t cumcnt[10] = {0};   /*  cumulative counts per digit  */

/* ensure that lead_cache contains leading digits for all k ≤ n */
static void ensure_lead(idx_t n)
{
    if (n + 1 <= lead_len) return;           /* already computed */

    /* extend capacity if needed */
    if (n + 1 > lead_cap) {
        size_t new_cap = (lead_cap == 0) ? 16 : lead_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(lead_cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);    /* memory failure not expected  */
        lead_cache = new_arr;
        lead_cap   = new_cap;
    }

    /* compute from current length up to n (inclusive) */
    for (size_t i = lead_len; i <= (size_t)n; ++i) {
        if (i == 0) {                      /* 0! = 1 */
            lead_cache[i] = 1;
            cumcnt[1]++;
        } else {
            log_sum += log10((double)i);
            double frac = log_sum - floor(log_sum);
            double power = pow(10.0, frac);
            int d = (int)(floor(power + 1e-12)); /* safeguard against rounding */
            if (d < 1) d = 1; else if (d > 9) d = 9;
            lead_cache[i] = (seq_t)d;
            cumcnt[d]++;
        }
    }
    lead_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_lead(n);
    int d = (int)lead_cache[(size_t)n];
    return (seq_t)cumcnt[d];
}
