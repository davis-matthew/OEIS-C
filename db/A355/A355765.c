/*
 * A355765  E.g.f. satisfies A(x)^2 * log(A(x)) = 1 - exp(-x*A(x)).
 * https://oeis.org/A355765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (n-2*k+1)^(k-1) *
 *            Stirling2(n,k)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *a_cache = NULL;          /* cached a(n) values */
static size_t a_cache_cap = 0;         /* capacity of a_cache */
static size_t a_cache_used = 0;        /* next unused index */

static seq_t *prev_row = NULL;         /* S(n-1,k) */
static size_t prev_row_len = 0;        /* length of prev_row */

static seq_t *curr_row = NULL;         /* S(n,k) */
static size_t curr_row_len = 0;        /* length of curr_row */

static idx_t   computed_max = 0;       /* highest n with a(n) cached */

/* ------------------------------------------------------------------ */
/* power for seq_t: base^exp (exp >= 0) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    if (exp == 0) return (seq_t)1;
    if (base == (seq_t)0) return (seq_t)0;
    if (base == (seq_t)1) return (seq_t)1;
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base = base * base;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* grow a_cache if needed                                               */
static void ensure_a_cache(size_t need)
{
    if (need < a_cache_cap) return;
    size_t newcap = a_cache_cap > 0 ? a_cache_cap * 2 : 64;
    while (newcap <= need) newcap <<= 1;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(a_cache, newcap * sizeof(seq_t)));
#else
    seq_t *tmp = realloc(a_cache, newcap * sizeof(seq_t));
#endif
    if (!tmp) exit(EXIT_FAILURE);          /* out of memory */
    a_cache = tmp;
    a_cache_cap = newcap;
}

/* ------------------------------------------------------------------ */
/* compute a(n) for all 0 <= n <= max_n                                 */
static void compute_up_to(idx_t max_n)
{
    for (idx_t n = computed_max + 1; n <= max_n; ++n) {
        /* allocate current row of size n+1 */
        size_t newlen = (size_t)n + 1;
        if (newlen > curr_row_len) {
#ifdef __cplusplus
            seq_t *tmp = reinterpret_cast<seq_t*>(realloc(curr_row, newlen * sizeof(seq_t)));
#else
            seq_t *tmp = realloc(curr_row, newlen * sizeof(seq_t));
#endif
            if (!tmp) exit(EXIT_FAILURE);
            curr_row = tmp;
            curr_row_len = newlen;
        }

        if (n == 0) {                     /* base case */
            curr_row[0] = (seq_t)1;
        } else {
            curr_row[0] = (seq_t)0;       /* S(n,0) = 0 for n>0 */

            /* compute S(n,k) for k=1..n using previous row */
            for (size_t k = 1; k <= n; ++k) {
                seq_t val = prev_row[k - 1];
                if (k < prev_row_len) val += (seq_t)k * prev_row[k];
                curr_row[k] = val;
            }
        }

        /* compute a(n) using current row */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= (size_t)n; ++k) {
            seq_t base = (seq_t)n - (seq_t)2 * (seq_t)k + (seq_t)1;
            seq_t power = pow_seq(base, (idx_t)k - 1);
            seq_t term = curr_row[k] * power;
            if (((n - k) & 1) == 1) term = -term;
            sum += term;
        }

        ensure_a_cache((size_t)n);
        a_cache[n] = sum;
        a_cache_used = (size_t)n + 1;

        /* swap prev_row and curr_row for next iteration */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
        prev_row_len = (size_t)n + 1;
    }
    computed_max = max_n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!a_cache) {
        /* initialise for n == 0 */
        ensure_a_cache(1);
        a_cache[0] = (seq_t)1;
        a_cache_used = 1;

        /* row for n = 0 */
        prev_row = (seq_t*)malloc(sizeof(seq_t));
        if (!prev_row) exit(EXIT_FAILURE);
        prev_row[0] = (seq_t)1;
        prev_row_len = 1;

        curr_row = NULL;
        curr_row_len = 0;
        computed_max = 0;
    }
    if (n > computed_max) compute_up_to(n);
    return a_cache[n];
}
