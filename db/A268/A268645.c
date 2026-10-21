/*
 * A268645  Union of the factorial numbers (A000142) and the double factorials
 *          numbers (A006882).
 * https://oeis.org/A268645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the sorted union of * factorial
 *            numbers n! (n≥1) and double factorial numbers n!! (n≥1)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the two prime streams
 * factorial (1!,2!,3!,…) and double‑factorial (1!!,3!!,5!!,… for odd
 * and 2!!,4!!,6!!,… for even).  It merges them in non‑decreasing order,
 * discarding duplicates, and stores the results in a static cache so
 * that repeated calls are fast.  The state is updated only when new
 * terms are needed, so calls may be random in order. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* cache of the merged sequence */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* factorial stream */
static seq_t fact_val      = 1;   /* current factorial value   */
static idx_t fact_next_n   = 2;   /* next multiplier for fact   */
static bool  fact_active   = true;

/* odd double factorial stream */
static seq_t odd_val       = 1;   /* current odd double factorial value */
static idx_t odd_next_n    = 3;   /* next odd multiplier (step 2)   */
static bool  odd_active    = true;

/* even double factorial stream */
static seq_t even_val      = 2;   /* current even double factorial value */
static idx_t even_next_n   = 4;   /* next even multiplier (step 2)   */
static bool  even_active   = true;

seq_t a(idx_t n)
{
    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n)
    {
        /* find the smallest active value */
        seq_t best = (seq_t)-1;   /* sentinel value (max)      */
        bool any = false;

        if (fact_active) {
            best = fact_val;
            any  = true;
        }
        if (odd_active) {
            if (!any || odd_val < best) best = odd_val;
            any = true;
        }
        if (even_active) {
            if (!any || even_val < best) best = even_val;
            any = true;
        }

        /* determine which streams produce this best value */
        bool sel_fact = fact_active && fact_val == best;
        bool sel_odd  = odd_active  && odd_val  == best;
        bool sel_even = even_active && even_val == best;

        /* ensure cache capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_ptr = seq_alloc_new_array(new_cap);
            if (new_ptr == NULL) abort();   /* no‑memory guard */
            for (size_t i = 0; i < cache_len; ++i)
                new_ptr[i] = cache[i];
            seq_free_array(cache);
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* store the selected value */
        cache[cache_len++] = best;

        /* advance selected streams */
        if (sel_fact) {
            if (sizeof(seq_t) <= 8) {
                __int128 prod = (__int128)fact_val * fact_next_n;
                if (prod > (uint64_t)-1) fact_active = false;
                else { fact_val = (seq_t)prod; fact_next_n++; }
            } else {
                fact_val = fact_val * fact_next_n;
                fact_next_n++;
            }
        }

        if (sel_odd) {
            if (sizeof(seq_t) <= 8) {
                __int128 prod = (__int128)odd_val * odd_next_n;
                if (prod > (uint64_t)-1) odd_active = false;
                else { odd_val = (seq_t)prod; odd_next_n += 2; }
            } else {
                odd_val = odd_val * odd_next_n;
                odd_next_n += 2;
            }
        }

        if (sel_even) {
            if (sizeof(seq_t) <= 8) {
                __int128 prod = (__int128)even_val * even_next_n;
                if (prod > (uint64_t)-1) even_active = false;
                else { even_val = (seq_t)prod; even_next_n += 2; }
            } else {
                even_val = even_val * even_next_n;
                even_next_n += 2;
            }
        }
    }

    return cache[(size_t)n - 1];
}
