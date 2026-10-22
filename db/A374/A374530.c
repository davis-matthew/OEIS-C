/*
 * A374530  Lexicographically earliest permutation of the nonnegative terms
 *          such that the absolute difference between the rightmost digit of
 *          a(n) and the leftmost digit of a(n+1) is the smallest possible
 *          one.
 * https://oeis.org/A374530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374530: the lexicographically earliest permutation of the
   nonnegative integers where the absolute difference between the
   rightmost digit of a(n) and the leftmost digit of a(n+1) is the
   smallest possible one.  The algorithm keeps a set of already used
   numbers and, at each step, chooses the smallest unused number whose
   leading digit equals the last digit of the previous number if such a
   number exists; otherwise it picks the smallest unused number whose
   leading digit differs by 1.  For difference 0 the choice is forced,
   for difference 1 it chooses the lexicographically smallest
   candidate. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_DIGITS 20          /* enough for 64‑bit numbers; grows only if seq_t is larger */

/* look‑up table of powers of ten (seq_t can be uint64_t or big integer) */
static seq_t pow10_tbl[MAX_DIGITS];

/* state of the generator for each leading digit */
struct DigitInfo {
    size_t len;       /* number of decimal digits of the current numbers */
    size_t suffix;    /* current suffix without the leading digit */
    bool exhausted;   /* whether all numbers with this leading digit are used */
};
static struct DigitInfo digit_info[10];

/* Return the smallest unused number that has the given leading digit.
   If none exists, return (seq_t)-1.  The state is NOT advanced. */
static inline seq_t peek_digit(int d)
{
    if (digit_info[d].exhausted) return (seq_t)-1;
    return (seq_t)d * pow10_tbl[digit_info[d].len - 1] + digit_info[d].suffix;
}

/* Consume the current smallest unused number with leading digit d,
   advance the state, and return the value. */
static inline seq_t consume_digit(int d)
{
    seq_t val = (seq_t)d * pow10_tbl[digit_info[d].len - 1] + digit_info[d].suffix;

    if (d == 0) {                 /* only 0 itself exists as a 0‑leading number */
        digit_info[d].exhausted = true;
        return val;
    }

    if (digit_info[d].len == 1 && digit_info[d].suffix == 0) {
        digit_info[d].len = 2;
        digit_info[d].suffix = 0;
    } else {
        seq_t max_suffix = pow10_tbl[digit_info[d].len - 1] - (seq_t)1;
        if (digit_info[d].suffix < max_suffix) {
            digit_info[d].suffix++;
        } else {
            digit_info[d].len++;
            digit_info[d].suffix = 0;
        }
    }
    return val;
}

/* The function that computes the sequence. */
seq_t a(idx_t n)
{
    /* initialise static tables on first call */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;

    if (!initialized) {
        initialized = true;

        /* powers of ten */
        pow10_tbl[0] = (seq_t)1;
        for (int i = 1; i < MAX_DIGITS; ++i)
            pow10_tbl[i] = pow10_tbl[i-1] * (seq_t)10;

        /* digit generators */
        for (int d = 0; d < 10; ++d) {
            digit_info[d].len = 1;
            digit_info[d].suffix = 0;
            digit_info[d].exhausted = false;
        }
    }

    /* generate terms until we have at least n terms cached */
    while (cache_len < n) {
        seq_t next_val;

        if (cache_len == 0) {                 /* first term is always 0 */
            next_val = 0;
            digit_info[0].exhausted = true;   /* 0 cannot be reused */
        } else {
            seq_t cur = cache[cache_len - 1];
            int last_digit = (int)(cur % (seq_t)10);

            /* prefer difference = 0 */
            seq_t cand0 = peek_digit(last_digit);
            if (cand0 != (seq_t)-1) {
                next_val = cand0;
                consume_digit(last_digit);
            } else {
                /* must take difference = 1: choose smallest candidate */
                int best_d = -1;
                seq_t best_val = (seq_t)-1;

                if (last_digit > 0) {
                    seq_t cand = peek_digit(last_digit - 1);
                    if (cand != (seq_t)-1 &&
                        (best_d == -1 || cand < best_val)) {
                        best_val = cand;
                        best_d = last_digit - 1;
                    }
                }
                if (last_digit < 9) {
                    seq_t cand = peek_digit(last_digit + 1);
                    if (cand != (seq_t)-1 &&
                        (best_d == -1 || cand < best_val)) {
                        best_val = cand;
                        best_d = last_digit + 1;
                    }
                }
                /* In normal operation best_d will always be found. */
                next_val = best_val;
                consume_digit(best_d);
            }
        }

        /* Mark the number as used.  We assume it fits in size_t for the
           indices we will ask for (n up to ~10000). */
        size_t idx = (size_t)next_val;
        if (idx >= used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 32;
            while (new_cap <= idx) new_cap <<= 1;
            bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
            if (!tmp) return (seq_t)-1;
            /* initialise new entries to false */
            for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
            used = tmp;
            used_cap = new_cap;
        }
        used[idx] = true;

        /* Store in cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)-1;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_val;
    }

    return cache[n-1];
}
