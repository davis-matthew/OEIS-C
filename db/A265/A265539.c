/*
 * A265539  Largest base-8 palindrome m <= n, written in base 10.
 * https://oeis.org/A265539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest base‑8 palindrome m <= n, returned as a decimal value
   (OEIS A265539).  The algorithm builds the base‑8 representation
   of n, mirrors its most significant half to obtain a palindrome
   candidate, and if that candidate exceeds n it decrements the
   prefix appropriately.  Leading zero after decrement collapses
   the length by one, giving the largest all‑7 palindrome of that
   smaller length.  Complexity O(log₈ n). */
#include <stdint.h>
#include <stdbool.h>

static seq_t pow8(idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        r *= (seq_t)8;
        e--;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Step 1: determine the number of octal digits of n */
    idx_t tmp = n;
    idx_t len = 0;
    while (tmp > 0) {
        len++;
        tmp /= 8;
    }

    /* Step 2: store the octal digits most significant first */
    int msb[64];          /* len <= 64 for all practical n */
    tmp = n;
    for (idx_t i = len; i > 0; --i) {
        msb[(int)i - 1] = (int)(tmp % 8);
        tmp /= 8;
    }

    /* Step 3: build the palindromic candidate from the prefix */
    int half = (len + 1) / 2;
    int pal[64];
    for (idx_t i = 0; i < len; ++i) {
        if (i < (idx_t)half)
            pal[(int)i] = msb[(int)i];
        else
            pal[(int)i] = msb[(int)len - 1 - (int)i];
    }

    /* Convert palindrome digits to decimal */
    seq_t cand = 0;
    for (idx_t i = 0; i < len; ++i)
        cand = cand * (seq_t)8 + (seq_t)pal[(int)i];

    /* If cand <= n, we are done */
    if (cand <= n) return cand;

    /* Step 4: decrement the left half (prefix) by 1, borrowing as needed */
    int borrow = 1;
    for (int i = half - 1; borrow && i >= 0; --i) {
        if (pal[i] > 0) {
            pal[i]--;
            borrow = 0;
        } else {
            pal[i] = 7;
        }
    }

    /* If the most significant digit became zero, the palindrome
       has only len-1 digits, all of them being 7. */
    if (pal[0] == 0) {
        return pow8((idx_t)(len - 1)) - (seq_t)1;
    }

    /* Step 5: mirror the updated prefix to the right side again */
    for (idx_t i = half; i < len; ++i)
        pal[(int)i] = pal[(int)len - 1 - (int)i];

    /* Convert the new palindrome to decimal */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i)
        result = result * (seq_t)8 + (seq_t)pal[(int)i];

    return result;
}
