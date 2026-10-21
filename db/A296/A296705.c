/*
 * A296705  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = nth positive integer such that in its base‑7
 *            representation the number of rises (#indices i with d(i) <
 *            d(i+1)) is strictly less than the number of falls (#indices i
 *            with d(i) > d(i+1))
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: check if a given number satisfies the rises/falls condition. */
static bool in_sequence(seq_t n)
{
    /* Extract base‑7 digits into an array (least significant first). */
    int digits[128];          /* log_7 of 2^128 ≈ 43, 128 is safe. */
    int len = 0;
    seq_t temp = n;

    if (temp == (seq_t)0)     /* 0 is not a natural number. */
        return false;

    while (temp > (seq_t)0) {
        seq_t q = temp / (seq_t)7;
        int r  = (int)(temp - q * (seq_t)7);  /* remainder 0..6, cast to int. */
        digits[len++] = r;
        temp = q;
    }

    /* Count rises and falls by scanning digits from most significant to least. */
    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        int hi = digits[i];
        int lo = digits[i - 1];
        if (hi < lo)  ++rises;
        else if (hi > lo)  ++falls;
    }
    return rises < falls;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = (seq_t)1;

    while (found < n) {
        if (in_sequence(x)) {
            ++found;
            if (found == n)
                return x;
        }
        x = x + (seq_t)1;   /* Increment to next candidate. */
    }
    /* Unreachable for positive n, but return a default value to satisfy the compiler. */
    return (seq_t)0;
}
