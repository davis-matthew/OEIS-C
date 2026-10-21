/*
 * A256656  Numbers for which the minimal alternating Fibonacci representation
 *          has positive trace.
 * https://oeis.org/A256656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer whose minimal alternating Fibonacci
 *            representation ends with a positive term
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------  Required helper headers & types ------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------  Function prototype provided by the build --------- */
seq_t a(idx_t n);

/* -------------  Internal utilities ------------------------------ */
#define INF 255U

/* -------------  Global static state ------------------------------ */
static bool initialized = false;
static seq_t *seq_values = NULL;        /* array of sequence terms */
static idx_t seq_len   = 0;             /* number of terms stored */

static seq_t *fib_arr = NULL;           /* Fibonacci numbers used in DP */
static size_t fib_cnt = 0;              /* count of Fibonacci numbers */

/* -------------  Compute Fibonacci numbers up to 2*LIMIT --------- */
static void compute_fibs(idx_t LIMIT)
{
    /* we generate until fib > 2*LIMIT to be safe */
    if (fib_arr) {
        free(fib_arr);
        fib_arr = NULL;
    }
    fib_cnt = 0;
    fib_arr = (seq_t *)malloc(64 * sizeof(seq_t));
    if (!fib_arr) return;           /* not expected to happen */

    fib_arr[0] = (seq_t)1;
    fib_arr[1] = (seq_t)2;
    fib_cnt = 2;
    while (fib_arr[fib_cnt-1] <= (seq_t)(2*LIMIT))
    {
        fib_arr[fib_cnt] = fib_arr[fib_cnt-1] + fib_arr[fib_cnt-2];
        fib_cnt++;
        if (fib_cnt >= 64) break;    /* safety for very large LIMIT */
    }
}

/* -------------  Dynamic programming to find minimal lengths ----- */
static void compute_sequence(idx_t LIMIT)
{
    /* free old data if any */
    if (seq_values) free(seq_values);
    seq_values = NULL;
    seq_len   = 0;

    compute_fibs(LIMIT);

    size_t size = 2 * LIMIT + 1;
    int64_t offset = LIMIT;           /* index of sum 0 */

    /* Allocate DP arrays */
    uint8_t *even_cur = (uint8_t *)malloc(size * sizeof(uint8_t));
    uint8_t *odd_cur  = (uint8_t *)malloc(size * sizeof(uint8_t));
    uint8_t *even_next = (uint8_t *)malloc(size * sizeof(uint8_t));
    uint8_t *odd_next  = (uint8_t *)malloc(size * sizeof(uint8_t));
    if (!even_cur || !odd_cur || !even_next || !odd_next) return;

    /* Initialize */
    for (size_t i = 0; i < size; ++i) {
        even_cur[i] = INF;
        odd_cur[i]  = INF;
    }
    even_cur[offset] = 0; /* no terms so far, parity even */

    /* Process Fibonacci indices from largest to smallest */
    for (size_t fi = fib_cnt; fi > 0; --fi)        /* fi is 1-based */
    {
        size_t i = fi - 1;                         /* current Fibonacci index */
        seq_t fib_val = fib_arr[i];

        /* copy current DP to next */
        for (size_t j = 0; j < size; ++j) {
            even_next[j] = even_cur[j];
            odd_next[j]  = odd_cur[j];
        }

        /* Transitions using current Fibonacci number */
        for (size_t s = 0; s < size; ++s) {
            uint8_t l_even = even_cur[s];
            if (l_even != INF) {
                int64_t ns = (int64_t)s + (int64_t)fib_val;
                if (0 <= ns && ns < (int64_t)size) {
                    uint8_t nl = l_even + 1;
                    if (nl < odd_next[ns]) odd_next[ns] = nl;
                }
            }
            uint8_t l_odd = odd_cur[s];
            if (l_odd != INF) {
                int64_t ns = (int64_t)s - (int64_t)fib_val;
                if (0 <= ns && ns < (int64_t)size) {
                    uint8_t nl = l_odd + 1;
                    if (nl < even_next[ns]) even_next[ns] = nl;
                }
            }
        }

        /* swap pointers for next iteration */
        uint8_t *tmp;
        tmp = even_cur;  even_cur  = even_next;  even_next  = tmp;
        tmp = odd_cur;   odd_cur   = odd_next;   odd_next   = tmp;
    }

    /* Allocate space for sequence values */
    seq_values = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!seq_values) {
        /* free DP arrays */
        free(even_cur); free(odd_cur); free(even_next); free(odd_next);
        return;
    }

    /* Build the sequence from minimal lengths */
    idx_t count = 0;
    for (idx_t val = 1; val <= LIMIT; ++val) {
        size_t idx = offset + val;
        uint8_t len_e = even_cur[idx];
        uint8_t len_o = odd_cur[idx];
        uint8_t min_len = (len_e <= len_o) ? len_e : len_o;
        if (min_len != INF && (min_len & 1U) == 1U) {  /* odd minimal length: positive trace */
            seq_values[count++] = (seq_t)val;
            if (count > LIMIT) break;                /* safety */
        }
    }
    seq_len = count;

    /* free DP arrays */
    free(even_cur); free(odd_cur); free(even_next); free(odd_next);
}

/* -------------  Ensure the sequence has at least n terms -------- */
static void ensure_sequence(idx_t n)
{
    if (!initialized || n > seq_len) {
        /* start with a reasonable limit, double until we have enough terms */
        idx_t limit = 200000;
        while (seq_len < n) {
            compute_sequence(limit);
            if (seq_len >= n) return;
            limit *= 2;
        }
        initialized = true;
    }
}

/* ---------------  Public function ------------------------------ */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_values[(size_t)(n - 1)];
}
