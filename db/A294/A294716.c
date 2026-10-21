/*
 * A294716  Numbers that are the sum of 2 squares in exactly 5 ways.
 * https://oeis.org/A294716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 34, u32 n <= 103, u64 n <= 103, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer that is the sum of two squares in exactly five
   unordered ways (including the representation with a = 0).  The function
   builds all such integers up to a fixed bound the first time it is called
   and returns the requested element. */

#include <stdlib.h>
#include <math.h>

/* Maximum bound to generate sequence terms.  Adjust if more terms are
   required. */
#define MAX_GENERATE 20000000u

static uint16_t *cnts = NULL;  /* representation counts      */
static seq_t   *vals = NULL;   /* list of numbers with 5 reps */
static size_t  vals_len = 0;
static size_t  vals_cap = 0;

/* Ensure storage for the vals array. */
static void ensure_vals_capacity(size_t needed)
{
    if (needed > vals_cap) {
        size_t new_cap = vals_cap ? vals_cap * 2 : 256;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        vals = new_ptr;
        vals_cap = new_cap;
    }
}

/* Generate the list of numbers up to MAX_GENERATE that have exactly
   five unordered representations as sum of two squares.  The count
   array uses uint16_t to keep memory small. */
static void generate_sequence(void)
{
    const idx_t N = MAX_GENERATE;
    cnts = (uint16_t *)calloc((size_t)(N + 1), sizeof(uint16_t));
    if (!cnts) abort();

    idx_t max_a = (idx_t)sqrt((double)N);
    for (idx_t a = 0; a <= max_a; ++a) {
        uint64_t a2 = (uint64_t)a * a;
        idx_t max_b = (idx_t)sqrt((double)(N - a2));
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t n = a2 + (uint64_t)b * b;
            /* Increment count of representations for this n. */
            if (cnts[n] < 0xFFFF) /* guard against overflow */
                ++cnts[n];
        }
    }

    /* Collect values with exactly 5 unordered representations. */
    for (idx_t n = 1; n <= N; ++n) {
        if (cnts[n] == 5) {
            ensure_vals_capacity(vals_len + 1);
            vals[vals_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    /* Initialize on first call. */
    if (!cnts) generate_sequence();

    /* Indexing starts at 1. */
    if (n == 0 || (size_t)n > vals_len)
        abort(); /* out of bounds */

    return vals[n - 1];
}
