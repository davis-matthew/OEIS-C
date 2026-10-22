/*
 * A370408  Lexicographically earliest sequence of positive integers such that
 *          no three equal terms appear at distinct indices that are the side
 *          lengths of a triangle.
 * https://oeis.org/A370408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 856, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Greedy construction of OEIS sequence A370408.
   a(1) = 1. For n > 1, a(n) is the smallest positive integer t such that
   no earlier pair (i,j) with a(i)=a(j)=t forces a forbidden interval covering n.
   The forbidden interval from a pair i<j is (j, i+j); any k with
   j < k < i+j cannot also have value t.
   Therefore a value t can be placed at position n iff the maximum
   forbidden upper bound for t is < n.
*/

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static idx_t max_n_computed = 0;      /* largest n computed so far      */
static seq_t *seq_arr = NULL;          /* 1‑based sequence array          */
static idx_t seq_cap = 0;              /* current capacity of seq_arr    */

/* last occurrence of each value (1‑based index), 0 if none yet   */
static idx_t *last_index = NULL;
/* maximum forbidden upper bound (i+j‑1) for each value, 0 if none */
static idx_t *forbidden_upper = NULL;
static size_t value_cap = 0;           /* current capacity of value arrays */

/* ensure the value arrays can address indices up to 'cap' (value index) */
static void ensure_value_cap(size_t cap)
{
    if (cap <= value_cap) return;
    size_t old_cap = value_cap;
    size_t new_cap = (value_cap ? value_cap * 2 : 128);
    while (new_cap < cap) new_cap *= 2;

    idx_t *new_last = (idx_t *)realloc(last_index, (new_cap + 1) * sizeof(idx_t));
    idx_t *new_forb = (idx_t *)realloc(forbidden_upper, (new_cap + 1) * sizeof(idx_t));
    if (!new_last || !new_forb) exit(1);          /* fatal on allocation failure */

    last_index = new_last;
    forbidden_upper = new_forb;

    for (size_t i = old_cap + 1; i <= new_cap; ++i) {
        last_index[i] = 0;
        forbidden_upper[i] = 0;
    }
    value_cap = new_cap;
}

/* ensure the sequence array can address indices up to 'cap' (seq index) */
static void ensure_seq_cap(idx_t cap)
{
    if (cap <= seq_cap) return;
    idx_t old_cap = seq_cap;
    idx_t new_cap = (seq_cap ? seq_cap * 2 : 256);
    while (new_cap < cap) new_cap *= 2;

    seq_t *new_seq = (seq_t *)realloc(seq_arr, (new_cap + 1) * sizeof(seq_t));
    if (!new_seq) exit(1);
    seq_arr = new_seq;
    seq_cap = new_cap;
    /* new entries will be filled when needed */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below offset, return 0 */
    if (n <= max_n_computed)        /* already computed */
        return seq_arr[n];

    for (idx_t i = max_n_computed + 1; i <= n; ++i) {
        ensure_seq_cap(i);          /* make sure seq_arr can hold i */

        for (idx_t val = 1; ; ++val) {
            ensure_value_cap((size_t)val);

            /* allowed if no forbidden interval reaches i */
            if (forbidden_upper[val] < i) {
                seq_arr[i] = (seq_t)val;

                if (last_index[val] == 0) {
                    /* first occurrence of this value */
                    last_index[val] = i;
                } else {
                    /* new pair with previous occurrence */
                    idx_t cand = last_index[val] + i - 1;
                    if (cand > forbidden_upper[val])
                        forbidden_upper[val] = cand;
                    last_index[val] = i;
                }
                break;  /* value assigned, proceed to next index */
            }
            /* otherwise try the next candidate value */
        }
    }
    max_n_computed = n;
    return seq_arr[n];
}
