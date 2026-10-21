/*
 * A297124  Numbers having an up-first zigzag pattern in base 3; see Comments.
 * https://oeis.org/A297124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generates a(n) – the n-th number with an up‑first zigzag pattern
   in base 3. Numbers have the form 12… in base 3, the first step is up
   (1<2).  After the prefix 12 each subsequent digit must differ from the
   previous one, but may be either up or down – no restriction on
   segment lengths.  The n‑th such number is obtained by deciding the
   remaining digits in lexicographic order. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is assumed to be >= 1.  Find the minimal length L of a base‑3
       representation that contains the n-th number.  For length L the
       total number of qualifying numbers is 2^(L-1)-1, the prefix 12
       contributes exactly 2^(L-2) numbers. */

    idx_t p = 1;               /* 2^k, starting with 2^0 */
    int   k = 0;               /* exponent of p */

    /* Find the smallest k such that 2^k >= n+1.  Then the length is
       L = k+1. */
    while (p < n + 1) {
        p <<= 1;
        k++;
    }
    int   L = k + 1;           /* minimal length of the desired number   */

    /* The offset of the desired number within all numbers of length L
       (0‑based).  For length L there are 2^(L-2) numbers, so the indices
       start at 2^(L-2). */
    idx_t offset = n - (p >> 1);   /* p == 2^k, so p>>1 == 2^(k-1) == 2^(L-2) */

    /* Build the number digit by digit, computing the value on the fly.
       Base‑3 digits are processed from most significant to least. */
    seq_t value = 1;               /* first digit '1'  */
    value = value * 3 + 2;         /* second digit '2' */
    int    prev   = 2;             /* digit already placed */
    int    rem    = L - 2;         /* digits still to choose   */

    while (rem > 0) {
        /* Two possible next digits, sorted increasingly. */
        int cand1, cand2;
        if (prev == 0) {   cand1 = 1; cand2 = 2; }
        else if (prev == 1) {cand1 = 0; cand2 = 2; }
        else {             cand1 = 0; cand2 = 1; }

        /* For each choice the number of completions of the remaining
           positions is 2^(rem-1).  This is independent of which candidate
           is chosen. */
        idx_t cnt_each = ((idx_t)1) << (rem - 1);

        if (offset < cnt_each) {
            value = value * 3 + cand1;
            prev  = cand1;
        } else {
            offset = offset - cnt_each;
            value = value * 3 + cand2;
            prev  = cand2;
        }
        rem--;
    }
    return value;
}
