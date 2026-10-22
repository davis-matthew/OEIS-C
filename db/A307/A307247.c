/*
 * A307247  Second digit in the expansion of n in Fraenkel's exotic ternary
 *          representation.
 * https://oeis.org/A307247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 985, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the second digit d₁ in the Fraenkel exotic ternary expansion
   of n‑1.  The expansion uses the numerators of successive convergents
   to √2 (1, 3, 7, 17, …), coefficients dᵢ∈{0,1,2}, and the rule
   "if dᵢ₊₁=2 then dᵢ=0".  The sequence a(n) is defined by a(n)=d₁
   for the representation of n‑1.  The digits are computed
   greedily from the largest coefficient downward.  No hard‑coded
   table is used and the function works for any n≥1. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *p_vals = NULL;     /* array of the pᵢ sequence */
static size_t  p_cap = 0;        /* allocated capacity of p_vals   */
static size_t  p_len = 0;        /* current number of elements     */

/* Ensure that p_vals has enough capacity for at least new_cap elements. */
static void grow_p_cap(size_t new_cap) {
    if (p_cap < new_cap) {
        seq_t *tmp = (seq_t*)realloc(p_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        p_vals = tmp;
        p_cap = new_cap;
    }
}

/* Initialise the p sequence with the first two terms. */
static void init_p_seq(void) {
    grow_p_cap(4);           /* spare a few extra slots */
    p_vals[0] = (seq_t)1;
    p_vals[1] = (seq_t)3;
    p_len = 2;
}

/* Extend p_vals until the last element is strictly greater than limit. */
static void ensure_p_up_to(seq_t limit) {
    if (p_len == 0) init_p_seq();
    while (p_len >= 2 && p_vals[p_len-1] <= limit) {
        seq_t next = (seq_t)2 * p_vals[p_len-1] + p_vals[p_len-2];
        grow_p_cap(p_len + 1);
        p_vals[p_len] = next;
        ++p_len;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* a(1)=0, n≥1 by definition */

    seq_t m = (seq_t)(n-1);               /* we need the representation of n-1 */

    ensure_p_up_to(m);

    /* Find the highest index k with p_vals[k] <= m. */
    size_t k;
    if (p_len == 0) return (seq_t)0;     /* should never happen */
    if (p_len == 1) k = 0; else k = p_len-1;
    while (k > 0 && p_vals[k] > m) --k;

    seq_t rem = m;
    int prev_digit = -1;          /* previous (higher) coefficient */
    seq_t d1 = 0;                 /* value to return */

    for (size_t i = k; i != (size_t)-1; --i) {
        int cur_d;
        if (prev_digit == 2) {
            cur_d = 0;
        } else {
            seq_t two_pi = (seq_t)2 * p_vals[i];
            if (rem >= two_pi)        cur_d = 2;
            else if (rem >= p_vals[i])cur_d = 1;
            else                       cur_d = 0;
        }

        if (i == 1) d1 = (seq_t)cur_d;

        rem -= (seq_t)cur_d * p_vals[i];
        prev_digit = cur_d;

        if (i == 0) break;          /* size_t underflow would go to full */
    }

    return d1;
}
