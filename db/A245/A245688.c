/*
 * A245688  Numbers n such that n^2 + 1 is the product of three distinct
 *          Fibonacci numbers > 1.
 * https://oeis.org/A245688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245688: numbers n such that n^2 + 1 is the product of three
   distinct Fibonacci numbers > 1.  The sequence is formed by merging
   the increasing list of even‑indexed Lucas numbers (starting with L8 =
   47) with the two fixed exceptional values 99 and 15075. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"      /* seq_alloc(size_t)   */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static bool   initialized = false;

/* fixed exceptional values */
static const seq_t exceptions[2] = { (seq_t)99, (seq_t)15075 };
static idx_t exception_idx = 0;

/* Lucas‑sequence state for generating even‑indexed Lucas numbers */
static seq_t lucas_prev_prev;   /* L_{i-2}   */
static seq_t lucas_prev;       /* L_{i-1}   */
static idx_t lucas_index;      /* current index of lucas_prev   */
static bool   lucas_val_valid = false;
static seq_t  next_lucas_val;

/* compute the next even‑indexed Lucas number and store it in next_lucas_val */
static void compute_next_even_lucas(void)
{
    while (true) {
        idx_t next_index = lucas_index + 1;
        seq_t next_val   = lucas_prev_prev + lucas_prev;  /* L_i = L_{i-2} + L_{i-1} */
        lucas_prev_prev  = lucas_prev;
        lucas_prev       = next_val;
        lucas_index      = next_index;
        if ((next_index & 1u) == 0u && next_index >= 8u) {
            next_lucas_val  = next_val;
            lucas_val_valid = true;
            break;
        }
    }
}

/* emit the next term of the sequence into seq_cache */
static void generate_next(void)
{
    if (!lucas_val_valid)
        compute_next_even_lucas();

    bool have_lucas = lucas_val_valid;
    bool have_exc   = exception_idx < 2;

    seq_t chosen;
    if (have_lucas && have_exc) {
        if (next_lucas_val <= exceptions[exception_idx]) {
            chosen = next_lucas_val;
            lucas_val_valid = false;          /* force recomputation next time */
        } else {
            chosen = exceptions[exception_idx++];
        }
    } else if (have_lucas) {
        chosen = next_lucas_val;
        lucas_val_valid = false;
    } else {  /* only exceptions remain – practically unreachable */
        chosen = exceptions[exception_idx++];
    }

    /* ensure cache capacity */
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (!seq_cache)
            seq_cache = seq_alloc(new_cap);
        else
            seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = chosen;
}

/* initialise static state once */
static void init_sequence(void)
{
    seq_cache   = NULL;
    seq_len     = 0;
    seq_cap     = 0;
    initialized = true;

    /* Lucas initial values: L0 = 2, L1 = 1 */
    lucas_prev_prev = (seq_t)2;
    lucas_prev      = (seq_t)1;
    lucas_index     = 1;          /* index of lucas_prev */
    lucas_val_valid = false;

    exception_idx = 0;
}

/* ensure we have at least n terms (1‑based indexing) */
static void ensure_sequence(idx_t n)
{
    if (!initialized)
        init_sequence();

    while ((size_t)n > seq_len)
        generate_next();
}

seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_cache[(size_t)n - 1];
}
