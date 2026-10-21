/*
 * A263715  Nonnegative integers that are the sum or difference of two
 *          squares.
 * https://oeis.org/A263715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth element of the set of nonnegative integers that are a
 *            sum or difference of two squares
 * Range:     u8 n <= 217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether m is a sum of two squares.  Return true iff every prime
   congruent to 3 (mod 4) occurs to an even exponent in the prime
   factorisation of m.  The function works for arbitrary-precision
   seq_t values up to the size used by the sequence. */
static bool is_sum_two_squares(seq_t m)
{
    if (m == 0) return true;

    /* Remove all factors of 2 – they do not affect the condition. */
    while ((m & 1) == 0) m >>= 1;

    for (seq_t p = 3; p <= m / p; p += 2) {
        if ((p % 4) == 3) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            if (exp & 1) return false;  /* odd exponent of a 3 mod 4 prime */
        }
    }

    /* Any remaining factor is prime.  If it is 3 (mod 4) its exponent is 1. */
    if (m > 1 && (m % 4) == 3) return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 – guard against accidental 0 */

    idx_t target = n - 1;          /* zero‑based index in the sequence   */
    idx_t cnt = 0;
    seq_t cand = 0;

    for (;;) {
        bool ok;

        /* Numbers not ≡ 2 (mod 4) are always representable as a difference
           of two squares (x² - y²).  For numbers ≡ 2 (mod 4), we need to
           check whether cand/2 is a sum of two squares. */
        if ((cand & 3) != 2) {
            ok = true;
        } else {
            ok = is_sum_two_squares(cand >> 1);
        }

        if (ok) {
            if (cnt == target) return cand;
            ++cnt;
        }
        ++cand;
    }
}
