/*
 * A309252  a(n) is the least number not in the sequence so far and whose
 *          absolute difference from a(n-1) is not in the sequence so far,
 *          with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A309252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1, a(2) = 2. For n > 2 * if n-1 is 1 or of the form
 *            7·2^k–3 (k ≥ 0), then a(n) is the least * candidate not in the
 *            sequence so far and whose absolute difference * from a(n-1) is
 *            also not in the sequence so far; * otherwise a(n) = a(n-1) + 3.
 *            This follows the empirical pattern * described i...
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *seq = NULL;      /* 1‑indexed array of terms */
static size_t seq_cap = 0;     /* allocated capacity (number of elements) */
static idx_t   seq_used = 0;   /* highest index that has been computed */

/* ensure that the array has enough capacity for indices up to 'need' */
static void ensure_capacity(size_t need)
{
    if (seq_cap > need) return;
    size_t new_cap = seq_cap ? seq_cap : 4;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();              /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

/* test whether value 'val' appears in seq[1..upto] */
static bool in_sequence(idx_t upto, seq_t val)
{
    for (idx_t i = 1; i <= upto; ++i)
        if (seq[i] == val) return true;
    return false;
}

/* true if x is a power of two (x > 0) */
static bool is_power_of_two(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* true if n is 1 or of the form 7·2^k – 3 (k ≥ 0) */
static bool is_special_n(idx_t n)
{
    if (n == 1) return true;
    idx_t v = n + 3;                  /* v = 7·2^k */
    if (v % 7 != 0) return false;
    v /= 7;
    return is_power_of_two(v);
}

/* public function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* unused but harmless */

    /* initialise storage on first call */
    if (!seq) {
        seq_cap = 4;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) abort();
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        seq_used = 2;
    }

    /* return cached value if already computed */
    if (n <= seq_used) return seq[n];

    /* compute terms up to n */
    for (idx_t i = seq_used + 1; i <= n; ++i) {
        seq_t prev = seq[i - 1];
        seq_t cand;

        if (is_special_n(i - 1)) {
            /* search for the least acceptable candidate */
            cand = (seq_t)1;
            for (;;) {
                seq_t diff = (cand > prev) ? (cand - prev) : (prev - cand);
                if (!in_sequence(i - 1, cand) &&
                    !in_sequence(i - 1, diff)) {
                    break;
                }
                cand = cand + 1;
            }
        } else {
            cand = prev + (seq_t)3;
        }

        ensure_capacity(i);
        seq[i] = cand;
        seq_used = i;
    }

    return seq[n];
}
