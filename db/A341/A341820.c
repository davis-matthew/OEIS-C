/*
 * A341820  Lexicographically earliest sequence of distinct terms > 0 such
 *          that the n-th digit of the sequence is present in the product
 *          a(n)*a(n+1).
 * https://oeis.org/A341820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 259, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   product_contains_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A341820 – Lexicographically earliest sequence of distinct terms >0
 *  such that the n‑th digit of the concatenated digit sequence is present
 *  in the decimal digits of a(n)·a(n+1).
 *
 *  The function implements memoised generation of the sequence.  The first
 *  term is 1, and each subsequent term is chosen as the smallest unused
 *  integer that yields a product containing the required digit.
 *  No external libraries are required beyond the standard headers.
 *
 *  The implementation follows the C11 and C++17 compatibility rules;
 *  dynamic memory is allocated via realloc and the result is cast to the
 *  appropriate type.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/product_contains_digit.h"

/* ------------------- generic reallocation helper ------------------- */
static void *grow_array(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : *cap;
        while (new_cap < need)
            new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        if (!ptr) abort();          /* out of memory – stop execution */
        *cap = new_cap;
    }
    return ptr;
}

/* ------------------- storage for the sequence --------------------- */
static seq_t *seq_array = NULL;  /* array of generated terms            */
static size_t   seq_len  = 0;     /* number of terms in seq_array       */
static size_t   seq_cap  = 0;     /* allocated capacity of seq_array    */

/* digits of the concatenated sequence (each element is 0..9) */
static uint8_t *digits     = NULL;
static size_t   digits_len = 0;   /* number of digits already stored   */
static size_t   digits_cap = 0;   /* allocated capacity                */

/* used markers – 0 for unused, 1 for used                              */
static uint8_t *used_small = NULL;
static size_t   used_cap   = 0;   /* allocated capacity for used_small */

/* check if a given integer value is already used                       */
static bool used_index(size_t v)
{
    if (v >= used_cap) return false;
    return used_small[v] != 0;
}

/* mark a given integer value as used                                    */
static void set_used_index(size_t v)
{
    if (v >= used_cap) {
        size_t new_cap = (used_cap == 0) ? 64 : used_cap;
        while (v >= new_cap)
            new_cap <<= 1;
        uint8_t *new_arr = (uint8_t*)realloc(used_small, new_cap * sizeof(uint8_t));
        if (!new_arr) abort();
        memset(new_arr + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
        used_small = new_arr;
        used_cap   = new_cap;
    }
    used_small[v] = 1;
}

/* append the decimal digits of v (in correct order) to the digits array */
static void append_digits(seq_t v)
{
    uint8_t rev[64];
    size_t rev_len = 0;
    if (v == 0) {
        rev[rev_len++] = 0;
    } else {
        while (v != 0) {
            rev[rev_len++] = (uint8_t)(v % (seq_t)10);
            v /= (seq_t)10;
        }
    }
    digits = (uint8_t*)grow_array(digits, &digits_cap,
                                  digits_len + rev_len, sizeof(uint8_t));
    for (size_t i = 0; i < rev_len; ++i)
        digits[digits_len++] = rev[rev_len - 1 - i];
}

/* ensure that the sequence has been generated up to index n (≥1)  */
static void ensure_sequence(idx_t n)
{
    if (n < 1) return;

    if (seq_len == 0) {               /* first call – initialise a(1)=1 */
        seq_array = (seq_t*)grow_array(seq_array, &seq_cap,
                                       1, sizeof(seq_t));
        seq_len   = 1;
        seq_array[0] = (seq_t)1;
        append_digits((seq_t)1);
        set_used_index(1);
    }

    while (seq_len < (size_t)n) {
        /* we are computing a(seq_len + 1) (1‑based indexing)           */
        seq_t prev = seq_array[seq_len - 1];         /* a(n-1)           */
        uint8_t required_digit = digits[seq_len - 1]; /* n‑th digit       */

        seq_t cand = 1;
        for (;;) {
            /* skip numbers already used */
            size_t cand_u = (size_t)(uint64_t)cand;
            if (used_index(cand_u)) { ++cand; continue; }

            if (product_contains_digit(prev, cand, required_digit)) {
                seq_array = (seq_t*)grow_array(seq_array, &seq_cap,
                                               seq_len + 1, sizeof(seq_t));
                seq_array[seq_len++] = cand;
                append_digits(cand);
                set_used_index(cand_u);
                break;
            }
            ++cand;
        }
    }
}

/* public interface ---------------------------------------------------- */
/* Return a(n) for the lexicographically earliest sequence described
 * above.  The function works for any order of calls to a(idx_t) and
 * uses memoisation to avoid recomputing already known terms.            */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for indices < 1 */
    ensure_sequence(n);
    return seq_array[n - 1];
}
