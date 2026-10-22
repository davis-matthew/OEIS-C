/*
 * A309325  Numbers that are the sum of two successive palindromes.
 * https://oeis.org/A309325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002113(n-1) + A002113(n) A002113(k) is the k‑th
 *            palindrome in base 10 (pal(0)=0)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

/* Return the k-th palindrome (k ≥ 0, pal(0)=0, pal(1)=1, …). */
static seq_t nth_pal(idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k < 10) return (seq_t)k;   /* 1 … 9 */

    /* Skip the first 10 palindromes (0‑9). */
    idx_t idx = k - 10;   /* position within the remaining palindromes */
    int L = 2;            /* current length of palindromes */

    while (1) {
        int half_len = (L + 1) / 2;                      /* ceil(L/2)          */
        idx_t cnt = 9ULL * (idx_t)(pow10((idx_t)(half_len - 1))); /* how many of this length */
        if (idx < cnt) break;
        idx -= cnt;
        ++L;
    }

    int half_len = (L + 1) / 2;
    seq_t start      = pow10((idx_t)(half_len - 1));     /* first number of this block */
    seq_t first_half = start + (seq_t)idx;

    /* Reverse the decimal representation of a number. */
    seq_t rev = 0;
    seq_t tmp = first_half;
    while (tmp) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    seq_t result;
    if (L % 2 == 0) {  /* even length: mirror the whole first_half */
        seq_t exp = pow10((idx_t)half_len);
        result = first_half * exp + rev;
    } else {           /* odd length: mirror first_half without its last digit */
        seq_t prefix = first_half / 10;
        seq_t rev_prefix = 0;
        tmp = prefix;
        while (tmp) {
            rev_prefix = rev_prefix * 10 + (tmp % 10);
            tmp /= 10;
        }
        seq_t exp = pow10((idx_t)(half_len - 1));
        result = first_half * exp + rev_prefix;
    }

    return result;
}

seq_t a(idx_t n)
{
    /* a(1) uses pal(0)+pal(1), so handle n ≥ 1.         */
    /* n is 1‑based in the OEIS offset.                  */
    seq_t p1 = nth_pal(n - 1);
    seq_t p2 = nth_pal(n);
    return p1 + p2;
}
