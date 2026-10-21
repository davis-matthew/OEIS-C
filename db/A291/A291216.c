/*
 * A291216  Padovan numbers that are also squares.
 * https://oeis.org/A291216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches Padovan numbers and keeps those that are perfect squares. */
/* a(1)=0, a(2)=1, a(3)=4, a(4)=9, a(5)=16, a(6)=49, ... */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

static seq_t *sqs   = NULL;          /* array of square Padovan numbers     */
static size_t sqs_cap = 0;           /* current capacity of the array      */
static size_t sqs_len = 0;           /* number of terms stored             */

static seq_t pad_a = 1, pad_b = 1, pad_c = 1; /* Padovan window P(k‑3),P(k‑2),P(k‑1)   */
static bool initialized = false;                /* state initialisation flag          */

seq_t a(idx_t n)
{
    /* n is 1‑based; the function guarantees a(n) for n≥1. */
    if (!initialized)
    {
        /* store 0 as the first term of the sequence */
        ensure_capacity(1, &sqs_cap, (void **)&sqs, sizeof(seq_t));
        sqs[0] = 0;
        sqs_len = 1;

        /* 1 is a Padovan number and a perfect square. */
        seq_t first = pad_a; /* pad_a == 1 */
        seq_t s = floor_sqrt(first);
        if (s * s == first)
        {
            ensure_capacity(sqs_len + 1, &sqs_cap, (void **)&sqs, sizeof(seq_t));
            sqs[sqs_len++] = first;
        }

        initialized = true;
    }

    /* Generate Padovan numbers until we have at least n square terms. */
    while (sqs_len < (size_t)n)
    {
        /* Next Padovan number: P(k) = P(k‑3) + P(k‑2)  (pad_a + pad_b) */
        seq_t next = pad_a + pad_b;

        /* Shift the sliding window for the next iteration. */
        pad_a = pad_b;
        pad_b = pad_c;
        pad_c = next;

        /* Test whether the new number is a perfect square. */
        seq_t s = floor_sqrt(next);
        if (s * s == next && next != sqs[sqs_len - 1])
        {
            ensure_capacity(sqs_len + 1, &sqs_cap, (void **)&sqs, sizeof(seq_t));
            sqs[sqs_len++] = next;
        }
    }

    return sqs[n - 1];
}
