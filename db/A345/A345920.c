/*
 * A345920  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum < 0.
 * https://oeis.org/A345920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th integer such that the k‑th composition in standard
 *            order (graded reverse‑lexicographic, A066099) has
 *            reverse‑alternating sum < 0. Computed by brute‑force search
 *            according to the definition
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* test whether k satisfies the property */
static bool has_reverse_alt_negative(idx_t k)
{
    int64_t sum = 0;          /* signed accumulator for the alternating sum */
    idx_t prev_pos = 0;       /* position of the previous 1‑bit (1‑indexed) */
    bool plus = true;         /* sign for the next difference: true -> '+', false -> '-' */

    idx_t pos = 1;            /* current bit position (1‑indexed) */
    for (idx_t kcopy = k; kcopy != 0; ++pos) {
        if (kcopy & 1U) {            /* 1‑bit at current position */
            idx_t diff = pos - prev_pos;
            if (plus)
                sum += diff;
            else
                sum -= diff;
            plus = !plus;           /* flip sign for next difference */
            prev_pos = pos;
        }
        kcopy >>= 1U;                /* advance to next bit */
    }

    return sum < 0;
}

/* a(n) – nth term of the sequence A345920 */
seq_t a(idx_t n)
{
    static seq_t *list = NULL;      /* dynamic array of found k’s */
    static size_t stored = 0;       /* number of terms stored */
    static size_t capacity = 0;     /* allocated capacity of list */
    static idx_t next_k = 1;        /* next k to test */

    if (list == NULL) {             /* first call: allocate storage */
        capacity = 128;
        list = mem_new(seq_t, capacity);
        stored = 0;
        next_k = 1;
    }

    /* extend list until we have at least n terms */
    while (stored < n) {
        if (has_reverse_alt_negative(next_k)) {
            if (stored == capacity) {            /* grow the array */
                capacity *= 2;
                list = mem_resize(seq_t, list, capacity);
            }
            list[stored++] = (seq_t)next_k;        /* store the qualifying k */
        }
        ++next_k;
    }

    return list[n - 1];  /* sequence is 1‑indexed */
}
