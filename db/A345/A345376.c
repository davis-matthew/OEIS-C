/*
 * A345376  Number of Companion Pell numbers m <= n.
 * https://oeis.org/A345376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 345, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the number of Pell‑Lucas (companion Pell) numbers ≤ n.
 *  The sequence of Pell‑Lucas numbers is generated with the recurrence
 *  Q₀ = 2, Q₁ = 2, Qₖ = 2·Qₖ₋₁ + Qₖ₋₂ for k ≥ 2.
 *  Each call expands the stored list as needed and counts how many
 *  values are ≤ n.  The list is kept sorted, with the duplicate 2
 *  at the beginning, so a simple linear scan suffices because the
 *  list is tiny (≈ log₃.⁴¹₄ n terms). */
#include <stdlib.h>

static seq_t *pell = NULL;   /* dynamic array of Pell‑Lucas numbers */
static size_t cap  = 0;      /* allocated capacity */
static size_t len  = 0;      /* number of elements stored */
static seq_t prev_val = 0;   /* Qₖ₋₂ */
static seq_t curr_val = 0;   /* Qₖ₋₁ */

/* Helper to grow the array when needed.  It doubles the capacity
 * (starting at 4) whenever the current array is full. */
static void grow_array(void)
{
    size_t new_cap = (cap > 0) ? cap * 2 : 4;
    seq_t *new_pell = (seq_t *)realloc(pell, new_cap * sizeof(seq_t));
    if (!new_pell) {                  /* allocation failure – abort */
        exit(1);
    }
    pell = new_pell;
    cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure we have the first two Pell‑Lucas numbers stored. */
    if (len == 0) {
        if (cap < 2) {
            /* allocate at least room for two elements */
            size_t init_cap = 4;
            pell = (seq_t *)malloc(init_cap * sizeof(seq_t));
            if (!pell) exit(1);
            cap = init_cap;
        }
        pell[0] = (seq_t)2;
        pell[1] = (seq_t)2;
        len = 2;
        prev_val = pell[0];
        curr_val = pell[1];
    }

    /* Extend the list until the last element exceeds n.  The loop
     * adds elements only when the current maximum is ≤ n, so we
     * guarantee that all numbers ≤ n are present. */
    while (pell[len - 1] <= (seq_t)n) {
        seq_t next = (seq_t)2 * curr_val + prev_val;  /* Qₖ = 2·Qₖ₋₁ + Qₖ₋₂ */
        if (len == cap) grow_array();
        pell[len++] = next;
        prev_val = curr_val;
        curr_val = next;
    }

    /* Count how many stored numbers are ≤ n.  The array is sorted
     * (except for the initial duplicate 2), so a linear scan is
     * sufficient and extremely fast, given the tiny size. */
    seq_t count = 0;
    for (size_t i = 0; i < len; ++i) {
        if (pell[i] <= (seq_t)n) {
            count++;
        } else {
            break;
        }
    }
    return count;
}
