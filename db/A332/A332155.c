/*
 * A332155  Numbers with palindromic Morse code A060109.
 * https://oeis.org/A332155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332155: Numbers with palindromic Morse code
   The number is constructed from its decimal digits d[0]..d[L-1]
   (least to most significant).  For all k,
   d[k] + d[L-1-k] ≡ 0 (mod 10).
   Only digits 0 and 5 give palindromic Morse code.
   The function enumerates numbers in ascending order by length
   and finds the nth term. */

#include <stdbool.h>
#include <stdint.h>

static inline seq_t build_number(int len, const int u[])
{
    int k = (len + 1) / 2;          /* number of upper-side digits  (middle included if odd) */
    seq_t res = 0;
    for (int i = 0; i < k; ++i)
        res = res * 10 + (seq_t)u[i];         /* upper half (most significant digits) */

    for (int i = k - 1; i >= 0; --i) {
        if ((len & 1) && i == k - 1)   /* skip the middle digit for odd length */
            continue;
        int comp = (10 - u[i]) % 10;  /* complementary lower digit */
        res = res * 10 + (seq_t)comp;
    }
    return res;
}

static bool gen_rec(int pos, int len, int k, int u[],
                    idx_t *remain, seq_t *result)
{
    for (int d = 0; d < 10; ++d) {
        if (pos == 0 && d == 0)          /* leading digit cannot be 0 */
            continue;
        if ((len & 1) && pos == k - 1 && !(d == 0 || d == 5))
            continue;                    /* middle digit can only be 0 or 5 if odd length */

        u[pos] = d;
        if (pos == k - 1) {              /* full prefix ready: produce the number */
            seq_t num = build_number(len, u);
            if (*remain == 1) {          /* found the nth term (after 0 and 5) */
                *result = num;
                return true;
            }
            --(*remain);
        } else {
            if (gen_rec(pos + 1, len, k, u, remain, result))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;   /* a(1) = 0 */
    if (n == 2) return 5;   /* a(2) = 5 */

    idx_t remain = n - 2;   /* count numbers after 0 and 5 */
    seq_t result = 0;
    int digits[10];         /* enough for maximum even length needed */

    for (int len = 2; ; ++len) {
        int k = (len + 1) / 2;
        if (gen_rec(0, len, k, digits, &remain, &result))
            return result;
    }
}
