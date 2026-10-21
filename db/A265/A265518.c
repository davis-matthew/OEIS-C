/*
 * A265518  a(n) = largest base-6 palindrome m <= n such that every base-6
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 6.
 * https://oeis.org/A265518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each n, list the base‑6 digits (least‑significant first).  
   For each possible palindrome length L from the number of digits down to 1,  
   choose the largest palindrome consistent with the digit‑wise bound
   d[i] <= min(n[i], n[L-1-i]) for every pair.  
   The first L for which the most‑significant digit is >0 (or L==1) gives the
   largest admissible palindrome.  The resulting digits are concatenated
   as decimal digits to form the value returned in seq_t.  */
#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) for OEIS A265518. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Extract base‑6 digits of n, least‑significant first. */
    idx_t digits[64];          /* enough for 64‑bit numbers in base 6 */
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 6;
        tmp /= 6;
    }

    /* Temporary array for candidate palindrome digits, also LSB‑first. */
    uint8_t cand[64];

    /* Try largest possible length first. */
    for (idx_t L = len; L > 0; --L) {
        for (idx_t i = 0; i < L; ++i) {
            idx_t j = L - 1 - i;
            uint8_t d = (digits[i] < digits[j]) ? (uint8_t)digits[i] : (uint8_t)digits[j];
            cand[i] = d;
            cand[j] = d;
        }
        /* Skip if msd is zero (except for the single‑digit zero case). */
        if (cand[L - 1] == 0 && L != 1)
            continue;

        /* Build the decimal representation from msd to lsd. */
        seq_t result = 0;
        for (idx_t k = L; k-- > 0; ) {
            result = result * 10 + cand[k];
        }
        return result;
    }

    /* Fallback – should never reach here because L==1 will always succeed. */
    return (seq_t)0;
}
