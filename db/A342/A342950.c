/*
 * A342950  7-smooth numbers not divisible by 10: positive numbers whose prime
 *          divisors are all <= 7 but do not contain both 2 and 5.
 * https://oeis.org/A342950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 320, u32 n <= 2141, u64 n <= 9700,
 *            u128 n <= 9700, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate 7‑smooth numbers (primes 2,3,5,7), filter out those
   divisible by 10, and keep the sequence in ascending order.  Each
   call returns the n‑th valid element (a(1) = 1).  The algorithm runs in
   O(n) time with a small, constant‑time overhead per element. */

#include <stdlib.h>
#include <stdbool.h>

/* Ensure that an array can hold at least 'needed' elements.  */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t ncap = *cap ? *cap : 4;
    while (ncap < needed) ncap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, ncap * sizeof(seq_t));
    *arr = tmp;
    *cap = ncap;
}

seq_t a(idx_t n)
{
    /* Handle an illegal index (just return 0).  */
    if (n == 0)
        return 0;

    /* Global, statically allocated state.  */
    static seq_t *smooth = NULL;   /* All 7‑smooth numbers in order.   */
    static size_t smooth_cap = 0, smooth_len = 0;
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    static seq_t *valid = NULL;    /* Filtered sequence (not divisible by 10). */
    static size_t valid_cap = 0, valid_len = 0;

    /* --------- Initialisation --------- */
    if (smooth_len == 0) {
        /* Allocate and seed the first 7‑smooth number. */
        ensure_capacity(&smooth, &smooth_cap, 4);
        smooth[0] = (seq_t)1;
        smooth_len = 1;
        i2 = i3 = i5 = i7 = 0;

        /* Allocate and seed the first valid number. */
        ensure_capacity(&valid, &valid_cap, 4);
        valid[0] = (seq_t)1;
        valid_len = 1;
    }

    /* If we already have enough valid numbers, return immediately. */
    if (valid_len >= n)
        return valid[n - 1];

    /* --------- Main generation loop --------- */
    while (valid_len < n) {
        /* Compute the next candidates from the current pointers. */
        seq_t two   = smooth[i2] * 2;
        seq_t three = smooth[i3] * 3;
        seq_t five  = smooth[i5] * 5;
        seq_t seven = smooth[i7] * 7;

        /* Pick the smallest candidate. */
        seq_t next = two;
        if (three < next) next = three;
        if (five  < next) next = five;
        if (seven < next) next = seven;

        /* Append the new 7‑smooth number. */
        ensure_capacity(&smooth, &smooth_cap, smooth_len + 1);
        smooth[smooth_len++] = next;

        /* Advance all indices that produced this value. */
        if (next == two)   ++i2;
        if (next == three) ++i3;
        if (next == five)  ++i5;
        if (next == seven) ++i7;

        /* If the number is not divisible by 10, store it. */
        if (next % 10 != 0) {
            ensure_capacity(&valid, &valid_cap, valid_len + 1);
            valid[valid_len++] = next;
            /* We can return early once we hit the required index. */
            if (valid_len == n)
                return next;
        }
    }

    /* Should never reach here for valid inputs. */
    return 0;
}
