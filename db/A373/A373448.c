/*
 * A373448  10^n-th binary palindrome.
 * https://oeis.org/A373448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A006995(10^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow2.h"

/* Reverse `len` bits of `val` (least‑significant first). */
static seq_t
rev_bits(seq_t val, idx_t len)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < len; ++i) {
        rev <<= 1;
        if ((val >> i) & 1)
            rev |= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* k = 10^n  (the index in A006995) */
    seq_t k = pow10(n);

    seq_t cumulative = 0;

    for (idx_t L = 1; ; ++L) {
        seq_t count;
        if (L == 1) {
            count = 2;                     /* 0 and 1 */
        } else {
            idx_t m = (L + 1) / 2;          /* ceil(L/2) */
            count = pow2(m - 1);            /* 2^(m-1) */
        }

        seq_t next_cumulative = cumulative + count;

        if (k <= next_cumulative) {
            /* k is within palindromes of length L */
            seq_t offset = k - cumulative - 1;  /* zero‑based */

            if (L == 1) {                    /* special case */
                return (offset == 0) ? 0 : 1;
            }

            idx_t m = (L + 1) / 2;
            seq_t min_first = pow2(m - 1);    /* smallest first half */
            seq_t first = min_first + offset;

            seq_t result;
            if ((L & 1) == 0) {              /* even length */
                seq_t rev = rev_bits(first, m);
                result = (first << m) | rev;
            } else {                         /* odd length */
                seq_t rev = rev_bits(first >> 1, m - 1);
                result = (first << (m - 1)) | rev;
            }
            return result;
        }

        cumulative = next_cumulative;
    }
}
