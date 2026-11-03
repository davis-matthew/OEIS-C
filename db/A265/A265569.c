/*
 * A265569  Smallest base-7 palindrome m >= n, written in base 7.
 * https://oeis.org/A265569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑7 palindrome m >= n, written in base‑7
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 74 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Generate all base‑7 palindromes of fixed length L.
   Recursively builds the first half of the palindrome.
   When a full palindrome is formed it is tested against target_n.
   If found, its decimal‑digits form is stored in *out_val and true is returned. */
static bool gen_half(int L, int halfLen, int pos, int *digits, seq_t target_n, seq_t *out_val)
{
    if (pos == halfLen) {
        /* Construct the full palindrome in an array */
        int full[64];
        for (int i = 0; i < halfLen; ++i) full[i] = digits[i];
        for (int i = halfLen; i < L; ++i)
            full[i] = full[L - 1 - i];

        /* Compute the base‑7 numeric value and its decimal digit representation */
        seq_t m = 0;
        seq_t dec = 0;
        for (int i = 0; i < L; ++i) {
            m = m * 7 + (seq_t)full[i];
            dec = dec * 10 + (seq_t)full[i];
        }

        if (m >= target_n) {
            *out_val = dec;
            return true;
        }
        return false;
    }

    int start = (pos == 0) ? 1 : 0;  /* leading zero is forbidden for length ≥ 2 */
    for (int d = start; d <= 6; ++d) {
        digits[pos] = d;
        if (gen_half(L, halfLen, pos + 1, digits, target_n, out_val))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t target_n = (seq_t)n;

    /* Single‑digit palindromes cover 0 … 6 directly */
    if (n <= 6)
        return target_n;

    /* Generate palindromes of increasing length until one meets the criterion */
    for (int L = 2; ; ++L) {
        int halfLen = (L % 2 == 0) ? (L / 2) : ((L + 1) / 2);
        int digits[64];
        seq_t out;
        if (gen_half(L, halfLen, 0, digits, target_n, &out))
            return out;
    }
}
