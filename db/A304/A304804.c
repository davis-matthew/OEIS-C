/*
 * A304804  Solution (b(n)) of the complementary equation a(n) = b(n) + b(4n)
 *          ; see Comments.
 * https://oeis.org/A304804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complementary sequences for OEIS A304804:
   b(n) = least new (smallest positive integer not yet used);
   a(n) = b(n) + b(4n).
   The sequence required is {b(n)} (the first 24 terms are 1,3,4,5,6,7,8,10,11,12,13,14,16,17,18,19,20,22,23,24,25,27,28,29,...).  */
/* (b0 = 1; a0 = 2 is implied but not part of the sequence returned.) */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* External linkage for the required function */
extern seq_t a(idx_t n);

/* Static data for memoisation */
static seq_t *b_arr = NULL;          /* array of b values              */
static size_t  cap   = 0;            /* current capacity (number of slots) */
static bool   *used  = NULL;         /* bitset of all numbers already used   */
static size_t  used_size = 0;        /* allocated size of 'used'             */
static idx_t   next_compute = 0;     /* next index of b_arr that has not yet been computed */
static idx_t   global_max_n = 0;     /* largest n requested so far          */

/* Ensure that the 'used' array can index 'val'.  Expands in powers of 2. */
static void ensure_used(idx_t val)
{
    if ((size_t)val < used_size) return;

    size_t new_size = used_size ? (used_size << 1) : 128;
    while (new_size <= (size_t)val) new_size <<= 1;

    bool *tmp = (bool *)realloc(used, new_size * sizeof(bool));
    if (!tmp) exit(1);                   /* fatal error */
    memset(tmp + used_size, 0, (new_size - used_size) * sizeof(bool));
    used = tmp;
    used_size = new_size;
}

/* Ensure capacity for b_arr up to index 'max_idx' (inclusive). */
static void ensure_capacity(idx_t max_idx)
{
    if ((size_t)(max_idx + 1) <= cap) return;

    size_t new_cap = cap ? (cap << 1) : 16;
    while (new_cap <= (size_t)(max_idx + 1)) new_cap <<= 1;

    seq_t *new_b = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_b) exit(1);                /* fatal error */
    memset(new_b, 0, new_cap * sizeof(seq_t));

    if (b_arr) {
        memcpy(new_b, b_arr, cap * sizeof(seq_t));
        free(b_arr);
    }
    b_arr = new_b;
    cap   = new_cap;
}

/* Main function computing the sequence a(n) = b(n). */
seq_t a(idx_t n)
{
    if (n > global_max_n) global_max_n = n;

    /* To compute b(n) we need b up to 4*n (for a part computation). */
    idx_t max_b_needed = n > 4*n ? n : 4*n;
    if (max_b_needed > global_max_n) max_b_needed = global_max_n; /* future safety */
    if (4 * global_max_n > max_b_needed) max_b_needed = 4 * global_max_n;

    if (next_compute <= max_b_needed) {
        ensure_capacity(max_b_needed);

        seq_t curr_mex = 1;

        for (idx_t i = next_compute; i <= max_b_needed; ++i) {
            /* Find least positive integer not yet used */
            while (true) {
                idx_t mx = (idx_t)curr_mex;
                ensure_used(mx);
                if (!used[mx]) break;
                ++curr_mex;
            }

            seq_t b_val = curr_mex;
            b_arr[i] = b_val;

            /* Mark b_val as used */
            idx_t ux = (idx_t)b_val;
            ensure_used(ux);
            used[ux] = true;
            ++curr_mex; /* advance for next step */

            /* If i is a multiple of 4, compute a(i/4) = b(i/4)+b(i)
               and mark it as used. */
            if ((i & 3) == 0) {
                idx_t n_pending = i >> 2;            /* i / 4 */
                seq_t a_val = b_arr[n_pending] + b_arr[i];
                idx_t ua = (idx_t)a_val;
                ensure_used(ua);
                used[ua] = true;
            }
        }
        next_compute = max_b_needed + 1;
    }

    return b_arr[n];
}
