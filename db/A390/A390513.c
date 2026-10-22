/*
 * A390513  Rectangular array, (U(n,k)), read by descending antidiagonals. Let
 *          S be the sequence of all 01 words in lexicographic order. Then
 *          U(n,k) is the position in A390512 of the index of the word
 *          0^(k-1)w(n), where w(n) is the n-th word in S that is 0 or has
 *          first letter 1.
 * https://oeis.org/A390513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A390513 – sequence defined via an array U(n,k) read by descending antidiagonals.
    Let s be the smallest integer with s*(s-1)/2 ≥ n.
    Then i = n - (s-1)*(s-2)/2   (row number, 1‑based)
         j = s - i               (column number, 1‑based)
    Let w(i) be the (i‑th) word of S – the binary representation of i−1
    (with w(1) = “0”).  Its length is
           len = 1                      if i = 1
           len = ⌊log₂(i‑1)⌋ + 1         otherwise
    The value a(n) is the index of the word 0^(j‑1) w(i) in the enumeration
    of all binary words by increasing length and lexicographic order:
            a(n) = 2^{(j-1)+len} – 1 + (i‑1)  */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find (i,j) from the coefficient n in descending antidiagonals. */
    idx_t s = 1;
    while ((unsigned __int128)s * (s - 1) / 2 < n) {
        ++s;
    }
    idx_t prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t i = n - prev;   /* row */
    idx_t j = s - i;      /* column */

    /* Length of w(i).  For i==1 the word is “0” (length 1). */
    idx_t m = i - 1;
    idx_t len;
    if (m == 0) {
        len = 1;
    } else {
        len = 0;
        idx_t t = m;
        while (t) {
            ++len;
            t >>= 1;
        }
    }

    /* Compute 2^{(j-1)+len} in the type seq_t and add the offset (i-1). */
    idx_t L = (j - 1) + len;
    seq_t pow2 = ((seq_t)1) << (size_t)L;
    seq_t result = pow2 - 1 + (seq_t)(i - 1);
    return result;
}
