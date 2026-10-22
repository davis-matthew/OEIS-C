/*
 * A352155  Numbers m such that the smallest digit in the decimal expansion of
 *          1/m is 1, ignoring leading and trailing 0's.
 * https://oeis.org/A352155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A352155:
   Numbers m for which the smallest digit in the decimal expansion
   of 1/m (ignoring leading and trailing zeros) is 1.
   The routine searches m sequentially, testing each candidate
   until the required number of terms has been found.  It uses
   only integer operations + - * / % and works for any of the
   supported seq_t types (fixed‑width or arbitrary‑precision). */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;     /* Dynamically grown array of terms */
static size_t terms_cap = 0;    /* Its allocated capacity */
static idx_t next_candidate = 1;/* Next m to test */
static idx_t term_count = 0;    /* Number of terms currently stored */

/* Ensure the array can hold at least 'need' elements. */
static void ensure_cap(idx_t need)
{
    if (terms_cap >= (size_t)need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < (size_t)need) new_cap = need;
    seq_t *tmp;
    if (terms == NULL) {
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    }
    terms = tmp;
    terms_cap = new_cap;
}

/* Return true iff m satisfies the definition of the sequence. */
static bool is_term(seq_t m)
{
    if (m == 1) return true;          /* 1/1 = 1.0 has minimal digit 1 */

    seq_t r = 1 % m;                  /* initial remainder */
    bool seen_nonzero = false;
    seq_t min_digit = 9;

    /* The repetition length is at most m-1; loop up to m times. */
    idx_t max_steps = (idx_t)m;
    idx_t steps = 0;
    while (steps < max_steps && r != 0) {
        ++steps;
        seq_t tmp = r * 10;
        seq_t d   = tmp / m;
        r        = tmp % m;

        if (!seen_nonzero) {
            if (d != 0) {
                seen_nonzero = true;
                if (d < min_digit) min_digit = d;
            }
            /* else skip leading zeros */
        } else {
            if (d < min_digit) min_digit = d;
        }

        if (min_digit == 0) return false;   /* a zero appears */
    }
    /* If we exited due to r==0 (finite decimal), all remaining
       digits are zero; they are ignored because trailing zeros
       are excluded.                                     */
    return (min_digit == 1);
}

/* ------------------------------------------------------------------ */
/* Public sequence interface                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined; safe guard */

    if (n <= term_count) return terms[n-1];

    ensure_cap(n);

    while (term_count < n) {
        if (is_term(next_candidate)) {
            terms[term_count++] = next_candidate;
        }
        ++next_candidate;
    }
    return terms[n-1];
}
