/*
 * A249068  a(n+1) gives the number of occurrences of the last digit of a(n)
 *          in octal base so far, up to and including a(n), with a(0)=0.
 * https://oeis.org/A249068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 402, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; for n ≥ 0,
   a(n+1) equals the number of occurrences of the last octal digit of a(n)
   in the octal representations of all terms a(0)…a(n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq = NULL;        /* dynamic array of computed terms */
static size_t seq_len = 0;       /* number of terms stored so far */
static size_t seq_cap = 0;       /* capacity of the seq array */
static seq_t counts[8];          /* counts of octal digits 0..7 */
static bool initialized = false;

/* add the octal digits of value `val` to the global `counts` */
static void add_digits(seq_t val)
{
    if (val == 0)
    {
        counts[0] += 1;
        return;
    }
    while (val > 0)
    {
        seq_t digit = val & 7;          /* last octal digit */
        counts[(size_t)digit] += 1;     /* use a normal index */
        val >>= 3;                      /* divide by 8 */
    }
}

/* ensure the seq array has space for at least `needed` elements */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* initialize the static data structures */
static void init(void)
{
    ensure_capacity(1);
    seq[0] = 0;
    seq_len = 1;
    for (int i = 0; i < 8; ++i)
        counts[i] = 0;
    add_digits(seq[0]);           /* counts[0] becomes 1 */
    initialized = true;
}

/* main sequence function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    /* generate terms up to index n (if not already present) */
    while (seq_len <= (size_t)n)
    {
        seq_t last_val   = seq[seq_len - 1];
        seq_t last_digit = last_val & 7;        /* last octal digit of a(n) */
        seq_t next_val   = counts[(size_t)last_digit]; /* occurrences so far */

        ensure_capacity(seq_len + 1);
        seq[seq_len] = next_val;
        seq_len++;

        add_digits(next_val);                  /* update counts with new term */
    }
    return seq[(size_t)n];
}
