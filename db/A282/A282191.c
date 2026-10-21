/*
 * A282191  Odd numbers divisible by 3 which are neither of the form p + 3^m +
 *          1 nor of the form p + 3^m - 1 with p prime.
 * https://oeis.org/A282191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation: odd multiples of 3 that are not of the
   form p + 3^m ± 1 for any prime p and integer m ≥ 0. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big() */

static seq_t *seq_arr = NULL;   /* array of computed terms          */
static idx_t  seq_size = 0;     /* number of stored terms           */
static idx_t  seq_cap  = 0;     /* current capacity of seq_arr      */

/* Ensure seq_arr can hold at least 'need' elements  */
static void grow_seq_arr(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_cap = new_cap;
}

/* Current candidate odd multiple of 3 (3, 9, 15, 21, ...) */
static seq_t candidate = 3;

/* Return the n-th term (n ≥ 1).  The sequence is 1‑indexed. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    grow_seq_arr(n);

    while (seq_size < n) {
        /* Test whether 'candidate' satisfies the definition  */
        bool bad = false;
        seq_t pow3 = 1;                /* 3^0 */
        while (pow3 < candidate) {
            seq_t diff = candidate - pow3;

            /* p = candidate - 3^m - 1  →  diff - 1  */
            if (diff >= 3) {
                seq_t p1 = diff - 1;
                if (is_prime_big(p1)) { bad = true; break; }
            }
            /* p = candidate - 3^m + 1  →  diff + 1  */
            if (diff >= 1) {
                seq_t p2 = diff + 1;
                if (is_prime_big(p2)) { bad = true; break; }
            }

            pow3 = pow3 * 3;
        }

        if (!bad) {
            seq_arr[seq_size++] = candidate;
        }

        /* Next odd multiple of 3   (increase by 6) */
        candidate += 6;
    }

    return seq_arr[n - 1];
}
