/*
 * A327239  For n >= 1, a(n) = b(n+1) - b(n) where b is A013947.
 * https://oeis.org/A327239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position(n+1) - position(n) where position(k) is * the
 *            position of the k-th “1” in the Kolakoski sequence. * The
 *            sequence is generated on demand; the algorithm keeps a * minimal
 *            linear‑time state so that any order of calls works
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *kolak = NULL;        /* holds both run lengths and output values */
static size_t kolak_cap = 0;       /* capacity of kolak array                */
static size_t kolak_len = 0;       /* number of elements actually written    */

static seq_t *ones = NULL;        /* positions of 1's in the output sequence */
static size_t ones_cap = 0;       /* capacity of ones array                 */
static size_t ones_len = 0;       /* number of positions stored             */

/* Current reading position for run lengths (the "p" index in the
 * traditional algorithm) and the current value to output ("current").
 * They are maintained permanently across calls. */
static size_t read_pos = 0;
static seq_t current = 1;

/* Grow an array to hold at least new_cap elements.  The array
 * elements are of type seq_t. */
static void grow_array(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        size_t newsize = *cap ? *cap * 2 : 16;
        if (newsize < new_cap)
            newsize = new_cap;
        *arr = (seq_t *)realloc(*arr, newsize * sizeof(seq_t));
        *cap = newsize;
    }
}

/* Initialise static state on the first call. */
static void init(void)
{
    if (kolak_len != 0)
        return;                           /* already initialised */

    grow_array(&kolak, &kolak_cap, 2);
    kolak[0] = 1;                         /* first run length  */
    kolak[1] = 2;                         /* second run length */
    /* No elements have been written yet, kolak_len = 0 */

    grow_array(&ones, &ones_cap, 16);
    ones_len = 0;
    read_pos = 0;
    current  = 1;
}

/* Generate the Kolakoski sequence further until we have at least
 * (n+1) positions of 1's stored (i.e. ones_len > n). */
static void ensure_positions(idx_t n)
{
    while (ones_len <= (size_t)n) {
        /* Read the next run length */
        idx_t run_length = (idx_t)kolak[read_pos];

        /* Generate the run */
        for (idx_t k = 0; k < run_length; ++k) {
            /* Grow the output array if required */
            grow_array(&kolak, &kolak_cap, kolak_len + 1);

            kolak[kolak_len] = current;

            /* If the value is 1, record its (1‑based) position */
            if (current == (seq_t)1) {
                grow_array(&ones, &ones_cap, ones_len + 1);
                ones[ones_len++] = (seq_t)(kolak_len + 1);
            }

            ++kolak_len;
        }

        /* Advance to the next run */
        current = 3 - current;          /* toggle between 1 and 2 */
        ++read_pos;
    }
}

/* Return the n-th term of sequence A327239.  The first term corresponds
 * to n = 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    init();
    ensure_positions(n);

    /* a(n) = position(n+1) - position(n) */
    return ones[(size_t)n] - ones[(size_t)n - 1];
}
