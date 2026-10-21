/*
 * A261679  Number of ordered pairs (u,v) of binary palindromes (see A006995)
 *          with u+v=n.
 * https://oeis.org/A261679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of ordered pairs (u,v) of binary palindromes
 * with u+v = n.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

static const seq_t ONE = (seq_t)1;

/* Reverse the lowest 'bits' bits of x.  */
static seq_t reverse_bits(seq_t x, unsigned bits)
{
    seq_t rev = 0;
    for (unsigned i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & ONE);
        x >>= 1;
    }
    return rev;
}

/* Build a binary palindrome from the first half 'fh'.
 * For odd length palindromes we drop the least significant bit of fh
 * when mirroring to avoid duplicating the middle bit. */
static seq_t make_palindrome(seq_t fh, unsigned m, bool odd)
{
    if (odd) {
        seq_t rev = reverse_bits(fh >> 1, m - 1);
        return (fh << (m - 1)) | rev;
    } else {
        seq_t rev = reverse_bits(fh, m);
        return (fh << m) | rev;
    }
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t seq_n = (seq_t)n;

    /* u = 0 is always a palindrome (binary "0") */
    if (is_palindrome(seq_n, 2))
        ++count;
    if (n == 0)
        return count;

    /* Determine maximum binary length needed.  */
    unsigned max_len = 0;
    seq_t tmp = 1;
    while (tmp <= seq_n) {
        ++max_len;
        tmp <<= 1;
    }

    /* Generate all binary palindromes <= n (excluding 0). */
    for (unsigned L = 1; L <= max_len; ++L) {
        unsigned m = (L + 1) / 2;
        seq_t fh_min = (ONE << (m - 1));     /* leading bit = 1 */
        seq_t fh_max = (ONE << m);           /* exclusive upper bound */
        /* Minimum palindrome of this length.  */
        seq_t min_pal = make_palindrome(fh_min, m, (L & 1));
        if (min_pal > seq_n)
            break;   /* any longer length will be > n */

/* iterate all first halves */
        for (seq_t fh = fh_min; fh < fh_max; ++fh) {
            seq_t u = make_palindrome(fh, m, (L & 1));
            if (u > seq_n)
                break;
            seq_t diff = seq_n - u;
            if (is_palindrome(diff, 2))
                ++count;
        }
    }

    return count;
}
