/*
 * A244008  Nonnegative integers with no repeated letters in their combined
 *          English decimal digit names.
 * https://oeis.org/A244008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, base, fini, full, easy
 * Formula:   a(n) = nth smallest nonnegative integer whose decimal digit *
 *            names concatenated contain no repeated letters
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* English names of the decimal digits */
static const char *digit_word[10] = {
    "zero",   "one",   "two",   "three",
    "four",   "five",  "six",   "seven",
    "eight",  "nine"
};

static uint64_t digit_mask[10];
static bool    digit_valid[10];

/* Buffer for all numbers that satisfy the property (expected < 20000). */
#define MAX_TERMS   20000
static seq_t term_buf[MAX_TERMS];
static size_t term_cnt = 0;
static bool   generated = false;

/* Comparator for qsort (compatible with C and C++). */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive backtracking:
 * value          – number built so far
 * used_digits    – bitmask of digits already used
 * used_letters   – bitmask of letters already used
 */
static void backtrack(seq_t value,
                      uint64_t used_digits,
                      uint64_t used_letters)
{
    if (term_cnt >= MAX_TERMS) return;            /* safety guard */

    for (int d = 0; d < 10; ++d) {
        if (!digit_valid[d]) continue;            /* digit with repeated letters   */
        if (used_digits & ((uint64_t)1 << d)) continue;     /* digit already used           */
        if (used_letters & digit_mask[d]) continue;          /* duplicate letter            */

        /* Form new number */
        seq_t new_val = value * (seq_t)10 + (seq_t)d;
        term_buf[term_cnt++] = new_val;

        /* Recurse with added digit */
        backtrack(new_val,
                  used_digits | ((uint64_t)1 << d),
                  used_letters | digit_mask[d]);
    }
}

/* One‑time generation of all valid numbers. */
static void generate_sequence(void)
{
    /* Compute letter masks and validity of each digit word */
    for (int d = 0; d < 10; ++d) {
        const char *s = digit_word[d];
        uint64_t mask = 0;
        bool dup = false;
        for (const char *p = s; *p; ++p) {
            char c = *p;
            if (c >= 'a' && c <= 'z') {
                uint64_t bit = 1ULL << (c - 'a');
                if (mask & bit) { dup = true; break; }
                mask |= bit;
            }
        }
        digit_mask[d]  = mask;
        digit_valid[d] = !dup;           /* true if the word has all distinct letters */
    }

    /* Enumerate all numbers */
    for (int d = 0; d < 10; ++d) {
        if (!digit_valid[d]) continue;                     /* skip digits with internal repeats */

        seq_t val = (seq_t)d;
        term_buf[term_cnt++] = val;                        /* single‑digit number */

        if (d == 0) continue;                              /* leading zero not allowed for multi‑digit */
        backtrack(val, ((uint64_t)1 << d), digit_mask[d]);
    }

    /* Sort the list increasingly */
    if (term_cnt > 1)
        qsort(term_buf, term_cnt, sizeof(seq_t), seq_compare);
}

/* OEIS entry */
seq_t a(idx_t n)
{
    if (!generated) {
        generate_sequence();
        generated = true;
    }
    if (n == 0 || (idx_t)n > term_cnt) return 0;   /* out of range -> 0 */
    return term_buf[n - 1];
}
