/*
 * A308659  a(1)=1; for n > 1, a(n) is the length of the longest palindromic
 *          suffix of (a(1), ..., a(n-1)).
 * https://oeis.org/A308659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest palindromic suffix of (a(1), … ,
 *            a(n-1)) The sequence is periodic with period 13
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file defines function a with external linkage. */
#include <stdbool.h>

static seq_t seq[14];          /* seq[1] .. seq[13] hold one period */
static bool seq_init = false;  /* one‐time initialization flag */

/* Build the first 13 terms by direct computation. */
static void init_seq(void)
{
    seq[1] = 1;
    for (idx_t i = 2; i <= 13; ++i) {
        seq_t best = 1;               /* at least the trivial palindrome of length 1 */
        for (idx_t k = 1; k <= i - 1; ++k) {
            /* check if the suffix of length k is a palindrome */
            bool pal = true;
            idx_t start = i - k;       /* first position of the suffix (1‑based) */
            for (idx_t off = 0; off < k / 2; ++off) {
                if (seq[start + off] != seq[i - 1 - off]) {
                    pal = false;
                    break;
                }
            }
            if (pal)
                best = k;              /* store longest found so far */
        }
        seq[i] = best;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (!seq_init) {
        init_seq();
        seq_init = true;
    }
    /* Periodic indexing: a(n) = seq[ ((n-1) % 13) + 1 ] */
    idx_t idx = ((n - 1) % 13) + 1;
    return seq[idx];
}
