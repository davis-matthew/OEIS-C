/*
 * A367738  a(n) = k is the smallest exponent k such that at least 3 equal
 *          decimal digits "n,n,n" appear in the decimal representation of 3^k
 *          (n=0,1,...,9).
 * https://oeis.org/A367738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = smallest k such that the decimal representation of 3^k
 *            contains the digit n three times consecutively
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static inline bool _has_triplet(const unsigned char *digits, size_t len, unsigned char d)
{
    if (len < 3) return false;
    for (size_t i = 0; i + 2 < len; ++i) {
        if (digits[i] == d && digits[i + 1] == d && digits[i + 2] == d)
            return true;
    }
    return false;
}

static inline void _multiply_by_three(unsigned char *digits, size_t *len)
{
    int carry = 0;
    for (size_t i = *len; i-- > 0; ) {
        int prod = digits[i] * 3 + carry;
        digits[i] = prod % 10;
        carry   = prod / 10;
    }
    if (carry) {
        /* shift digits right by one */
        for (size_t i = *len; i > 0; --i)
            digits[i] = digits[i - 1];
        digits[0] = carry;
        ++*len;
    }
}

static bool _computed = false;
static seq_t _answer[10];

seq_t a(idx_t n)
{
    if (!_computed) {
        /* compute all ten values once */
        unsigned char digits[500];
        size_t len = 1;
        digits[0] = 1;               /* 3^0 = 1 */
        bool found[10] = {false};
        size_t remaining = 10;
        seq_t k = 0;

        while (remaining) {
            /* increase exponent to k+1 */
            ++k;
            _multiply_by_three(digits, &len);

            for (int d = 0; d < 10; ++d) {
                if (!found[d] && _has_triplet(digits, len, (unsigned char)(d))) {
                    _answer[d] = (seq_t)k;
                    found[d] = true;
                    --remaining;
                }
            }
        }
        _computed = true;
    }

    if (n < 10)
        return _answer[n];
    else
        return 0;   /* undefined for n ≥ 10 */
}
