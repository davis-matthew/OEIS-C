/*
 * A383815  Palindromic primes in A380943.
 * https://oeis.org/A383815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth palindromic prime that can be expressed as a
 * concatenation of two primes p and q such that the concatenation q||p
 * is also prime.  The sequence is enumerated by testing all palindromes
 * in increasing order.
 */

#include <stdbool.h>
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"    /* int digit_len(seq_t n); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

/* Return true if val (already known to be prime) can be split as
   described.  len is the number of decimal digits of val. */
static bool has_concat_property(seq_t val, int len)
{
    for (int split = 1; split < len; ++split) {
        seq_t powRight = pow10((idx_t)(len - split));
        seq_t p = val / powRight;        /* left part */
        seq_t q = val % powRight;        /* right part */

        /* reject splits that would introduce leading zeros */
        if (digit_len(p) != split) continue;
        if (digit_len(q) != len - split) continue;

        if (!is_prime_big(p) || !is_prime_big(q)) continue;

        seq_t rev_concat = q * pow10((idx_t)digit_len(p)) + p;
        if (is_prime_big(rev_concat)) return true;
    }
    return false;
}

/* Reverse the decimal digits of x, given that it has d digits. */
static seq_t reverse_digits(seq_t x, int d)
{
    seq_t r = 0;
    for (int i = 0; i < d; ++i) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    idx_t found = 0;                    /* terms already found */

    for (int len = 3; ; ++len) {        /* minimal palindromic prime length */
        int half = len / 2;

        /* left part must have 'half' digits and the leading digit non‑zero */
        seq_t left_start = pow10((idx_t)(half - 1));
        seq_t left_end   = pow10((idx_t)half) - 1;
        idx_t count_left = (idx_t)(9 * pow10((idx_t)(half - 1)));

        for (idx_t i = 0; i < count_left; ++i) {
            seq_t left = left_start + (seq_t)i;
            seq_t rev_left = reverse_digits(left, half);

            if (len % 2 == 0) {            /* even length: no middle digit */
                seq_t val = left * pow10((idx_t)half) + rev_left;
                if (is_prime_big(val) &&
                    has_concat_property(val, len)) {
                    ++found;
                    if (found == n) return val;
                }
            } else {                       /* odd length: middle digit 0‑9 */
                seq_t left_pow = pow10((idx_t)(half + 1));
                seq_t mid_pow = pow10((idx_t)half);
                for (int mid = 0; mid <= 9; ++mid) {
                    seq_t val = left * left_pow
                              + (seq_t)mid * mid_pow
                              + rev_left;
                    if (is_prime_big(val) &&
                        has_concat_property(val, len)) {
                        ++found;
                        if (found == n) return val;
                    }
                }
            }
        }
    }
}
