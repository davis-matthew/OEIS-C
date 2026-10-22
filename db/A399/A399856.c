/*
 * A399856  a(n) is the least proper multiple of n whose factorial base
 *          expansion contains that of n as a substring.
 * https://oeis.org/A399856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 120, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399856 – least proper multiple of n whose factorial base
   expansion contains that of n as a substring.  The function
   a(n) = n * A399855(n) is computed by brute‑force search.
   For n >= 1 it returns the correct term; for n == 0 it returns 0
   (the offset of the OEIS table is 1). */
#include <stddef.h>
#include <stdbool.h>

/* Maximum number of factorial base digits needed for the values of
   interest (n <= 10 000 guarantees < 20 digits; 64 is therefore safe). */
#define MAX_DIGITS 64

/* Convert x to factorial base representation.
   digits[0] holds the coefficient of 1!, digits[1] of 2!, etc.
   len is set to the number of non‑zero digits written.  The caller
   must provide an array of at least MAX_DIGITS entries. */
static void factoradic(seq_t x, unsigned int digits[], size_t *len)
{
    size_t i = 0;
    seq_t rem = x;

    while (rem > 0 && i < MAX_DIGITS) {
        seq_t base = (seq_t)(i + 2);          /* divisor = (i+1)+1 = i+2 */
        unsigned int d = (unsigned int)(rem % base);
        digits[i++] = d;
        rem /= base;
    }
    *len = i;
}

/* Test whether the factorial‑base digit sequence of n appears as a
   contiguous block inside that of m.
   Both sequences are given in ascending order (least significant first).
   The result is true if there exist indices i such that m_high[i..i+len_n-1]
   equals n_high[0..len_n-1] (high to low). */
static bool contains_substring(const unsigned int n_digits[], size_t len_n,
                               const unsigned int m_digits[], size_t len_m)
{
    if (len_m < len_n)
        return false;

    /* Reverse both sequences to high‑to‑low order in temporary arrays.
       The arrays are small (<= MAX_DIGITS) so a fixed stack buffer is fine. */
    unsigned int rev_n[MAX_DIGITS];
    unsigned int rev_m[MAX_DIGITS];
    for (size_t i = 0; i < len_n; i++)
        rev_n[i] = n_digits[len_n - 1 - i];
    for (size_t i = 0; i < len_m; i++)
        rev_m[i] = m_digits[len_m - 1 - i];

    /* Slide rev_n over rev_m and check for equality. */
    for (size_t start = 0; start + len_n <= len_m; start++) {
        bool ok = true;
        for (size_t j = 0; j < len_n; j++) {
            if (rev_m[start + j] != rev_n[j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for offset 1 */

    seq_t n_seq = (seq_t)n;

    /* Factorial base digits of n. */
    unsigned int digits_n[MAX_DIGITS];
    size_t len_n = 0;
    factoradic(n_seq, digits_n, &len_n);

    /* Search for the smallest k >= 2 such that (k*n) contains n
       as a substring in factorial base expansion. */
    for (seq_t k = 2; ; ++k) {
        seq_t m = n_seq * k;

        unsigned int digits_m[MAX_DIGITS];
        size_t len_m = 0;
        factoradic(m, digits_m, &len_m);

        if (contains_substring(digits_n, len_n, digits_m, len_m))
            return m;                       /* found a(n) */
    }
}
