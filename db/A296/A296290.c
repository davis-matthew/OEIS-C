/*
 * A296290  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n-1), where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   in_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A296290: a(n) complementary to b(n) via
     a(0)=1, a(1)=4
     b(0)=2, b(1)=3
     a(n)=a(n-1)+a(n-2)+ n*b(n-1)   (n≥2)
     a and b are increasing complementary sequences
   The function returns the required a(n) for any n≥0
   (fast for n up to about 10 000).  No external libraries are needed.
*/

#include <stddef.h>   /* size_t */
#include "../utils/in_a.h"

#define MAXN 20000  /* sufficient for n up to about 10 000 */

/* static storage for the sequences */
static seq_t a_arr[MAXN + 1];
static seq_t b_arr[MAXN + 1];
static size_t a_len = 0;  /* number of a terms computed so far */
static size_t b_len = 0;  /* number of b terms computed so far */
static seq_t next_candidate = (seq_t)0;   /* next integer to try for b */

/* compute a(n) and b(n) up to target (inclusive) */
static void build_to(idx_t target) {
    if ((idx_t)a_len > target) return;   /* already sufficient */

    /* initial values */
    if (a_len == 0) {
        a_arr[0] = (seq_t)1;
        a_arr[1] = (seq_t)4;
        a_len = 2;

        b_arr[0] = (seq_t)2;
        b_arr[1] = (seq_t)3;
        b_len = 2;

        next_candidate = b_arr[1] + (seq_t)1;   /* start after last known b */
    }

    /* extend sequences */
    for (size_t k = a_len; k <= (size_t)target; ++k) {
        /* ensure b[k-1] exists */
        while (b_len <= k - 1) {
            /* find smallest integer > previous b that is not in a */
            while (in_a(a_arr, a_len, next_candidate)) {
                ++next_candidate;
            }
            b_arr[b_len++] = next_candidate;
            ++next_candidate;   /* next search will start after this value */
        }

        /* compute a[k] = a[k-1] + a[k-2] + k*b[k-1] */
        seq_t term = a_arr[k - 1] + a_arr[k - 2] + (seq_t)k * b_arr[k - 1];
        a_arr[k] = term;
        ++a_len;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n) {
    build_to(n);
    return a_arr[(size_t)n];
}
