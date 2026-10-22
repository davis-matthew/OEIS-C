/*
 * A352603  a(1) = 2; a(2) = 3; for n >= 3, a(n) = a(n-1) - (gpf(n-2) +
 *          gpf(n-1)) if a(n) >= 2 and has not previously appeared, otherwise
 *          a(n) = a(n-1) + (gpf(n-2) + gpf(n-1)), where gpf(k) = A006530(k) =
 *          greatest prime dividing k.
 * https://oeis.org/A352603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=3, for n>=3,
   a(n) = a(n-1)-(gpf(n-2)+gpf(n-1)) if that value is >=2
          and has not occurred before; otherwise
          a(n) = a(n-1)+(gpf(n-2)+gpf(n-1)). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gpf.h"

static seq_t *seq = NULL;      /* 1‑based array of the sequence */
static size_t seq_cap = 0;     /* capacity of the array (in seq_t units) */
static idx_t  seq_len = 0;     /* highest index that has been computed */

/* Ensure the array can address indices up to n (inclusive). */
static void ensure_seq_capacity(idx_t n)
{
    if (n < seq_cap)               /* current capacity already enough */
        return;

    /* New capacity: need at least n+1 entries (index 0 unused). */
    size_t new_cap = (size_t)n + 1;
    if (seq) {
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
    } else {
        seq = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!seq) abort();
    }
    seq_cap = new_cap;
}

/* Initialise the first two terms if they are not yet set. */
static void init_terms(void)
{
    if (seq_len >= 2)
        return;
    ensure_seq_capacity(2);
    seq[1] = (seq_t)2;
    seq[2] = (seq_t)3;
    seq_len = 2;
}

/* Check whether val occurs in seq[l..r] (both inclusive). */
static bool seen_before(idx_t l, idx_t r, seq_t val)
{
    for (idx_t i = l; i <= r; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; 0 is an invalid request */
        return (seq_t)0;

    init_terms();         /* make sure a(1) and a(2) exist */

    if (n <= seq_len)
        return seq[n];    /* already computed */

    ensure_seq_capacity(n);

    for (idx_t i = seq_len + 1; i <= n; ++i)
    {
        uint64_t g1 = gpf((uint64_t)seq[i - 2]);   /* gpf of a(i‑2)   */
        uint64_t g2 = gpf((uint64_t)seq[i - 1]);   /* gpf of a(i‑1)   */
        seq_t step = (seq_t)g1 + (seq_t)g2;

        seq_t next = seq[i - 1] + step;   /* optimistic addition   */
        bool subtract = false;

        /* Try subtraction only if it does not underflow. */
        if (seq[i - 1] >= step)
        {
            seq_t cand = seq[i - 1] - step;
            if (cand >= (seq_t)2 && !seen_before(1, i - 1, cand))
            {
                next = cand;
                subtract = true;
            }
        }

        seq[i] = next;
    }

    seq_len = n;
    return seq[n];
}
