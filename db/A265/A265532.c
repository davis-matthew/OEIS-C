/*
 * A265532  Largest base-4 palindrome m <= n, written in base 4.
 * https://oeis.org/A265532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest base‑4 palindrome m ≤ n, where the result is
 * returned as the decimal integer formed by the base‑4 digits of m.
 *
 * The algorithm computes the base‑4 representation of n, mirrors
 * the left half to form a candidate palindrome, and if the candidate
 * exceeds n it decrements the left half (including the middle digit
 * when the length is odd).  If this decrement would create a leading
 * zero, the largest palindrome of one fewer digit (all 3’s) is used.
 * The final digit string is then interpreted both as a base‑4
 * integer (to find the numeric value) and as a decimal integer to
 * return.
 *
 * Complexity: O(L) per call, where L is the number of base‑4 digits
 * of n (at most 32 for 64‑bit values).
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* n == 0 */
    if (n == 0) return (seq_t)0u;

    /* Step 1: obtain base‑4 digits of n, most significant first */
    int digits[32], len = 0;
    idx_t temp = n;
    int rev[32]; /* temporary reverse storage */
    int revlen = 0;
    while (temp) {
        rev[revlen++] = temp % 4;
        temp /= 4;
        if (revlen >= 32) break;    /* safety, should not happen */
    }
    for (int i = revlen - 1; i >= 0; --i) {
        digits[len++] = rev[i];
    }

    /* Step 2: form candidate palindrome by mirroring left half */
    int cand[32];
    memcpy(cand, digits, len * sizeof(int));
    for (int i = 0; i < len / 2; ++i)
        cand[len - 1 - i] = cand[i];

    /* Helper to compare two digit arrays lexicographically */
    int cmp = 0;
    for (int i = 0; i < len; ++i) {
        if (cand[i] > digits[i]) { cmp = 1; break; }
        if (cand[i] < digits[i]) { cmp = -1; break; }
    }

    if (cmp <= 0) { /* candidate ≤ n */
        /* convert candidate to the required output */
        seq_t dec = 0, dig = 0;
        for (int i = 0; i < len; ++i) {
            dec = dec * 4 + cand[i];
            dig = dig * 10 + cand[i];
        }
        (void)dec; /* unused but shows value if needed */
        return dig;
    }

    /* Step 3: decrement the left half (including middle) */
    int left[32];
    memcpy(left, digits, len * sizeof(int));
    int pos = (len - 1) / 2;            /* index of middle or last left digit */
    int carry = 1;
    for (int i = pos; i >= 0 && carry; --i) {
        int d = left[i] - carry;
        if (d < 0) {
            left[i] = 3; /* base‑4 maximum digit */
            carry = 1;
        } else {
            left[i] = d;
            carry = 0;
        }
    }

    if (left[0] == 0) {
        /* leading zero -> use largest palindrome of length len-1 (all 3's) */
        len -= 1;
        for (int i = 0; i < len; ++i) left[i] = 3;
    } else {
        /* mirror left half to right half */
        for (int i = 0; i < len / 2; ++i)
            left[len - 1 - i] = left[i];
    }

    /* Step 4: convert final digits to output */
    seq_t dec = 0, dig = 0;
    for (int i = 0; i < len; ++i) {
        dec = dec * 4 + left[i];
        dig = dig * 10 + left[i];
    }
    (void)dec; /* unused */
    return dig;
}
