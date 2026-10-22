/*
 * A348910  a(n) is the "real" part of f(n) = Sum_{k>=0, d_k>0} w^(d_k-1) *
 *          (-2)^k where Sum_{k>=0} d_k * 4^k is the base-4 representation of
 *          n and w = -1/2 + sqrt(-3)/2 is a primitive cube root of unity;
 *          sequence A348911 gives "w" parts.
 * https://oeis.org/A348910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k} ((d_k==1)?1:((d_k==3)?-1:0)) * (-2)^k , * where d_k
 *            are the base‑4 digits of n. This is the required * “real” part
 *            of the Eisenstein‑integer mapping
 * Range:     i8 n <= 9269, i16 n <= 9269, i32 n <= 9269, i64 n <= 9269,
 *            i128 n <= 9269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Function: a
 *
 * Description:
 *   Computes the "real" part of the Eisenstein‑integer mapping
 *   defined in OEIS A348910.  For a non‑negative integer n let
 *   n = Σ d_k · 4^k (base‑4 expansion, digits d_k ∈ {0,1,2,3}).
 *   The mapping is
 *        f(n) = Σ_{k, d_k>0} w^(d_k-1) · (-2)^k,
 *   where w = -½ + ½ √‑3 is a primitive cube root of unity.
 *
 *   In the Eisenstein system every element can be written uniquely
 *   as u + v w with integers u,v.  The integer u is called the
 *   “real” part.  For the powers of w we have
 *
 *          w^0 =  1  = 1 + 0 w     → a =  1
 *          w^1 =  w         = 0 + 1 w → a =  0
 *          w^2 = -1 - w     = -1 - 1 w → a = –1
 *
 *   Hence each non‑zero digit contributes
 *          +(-2)^k   if d_k = 1
 *          –(-2)^k   if d_k = 3
 *          0         if d_k = 2
 *
 *   The routine iterates over the base‑4 digits of n, accumulates
 *   the alternating powers of –2, and returns the resulting sum.
 *
 * Complexity:
 *   O(log₄ n) time and O(1) extra space.
 *
 *********************************************************************/

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;    /* (-2)^0 */
    idx_t x = n;

    while (x != 0)
    {
        unsigned digit = (unsigned)(x & 3u);   /* n % 4 */
        if (digit == 1)
            result += power;          /* +(-2)^k */
        else if (digit == 3)
            result -= power;          /* -(-2)^k */
        /* digit == 2 gives 0 contribution; digit == 0 is skipped */

        power = power * (seq_t)(-2);
        x >>= 2;                      /* shift to next base‑4 digit */
    }
    return result;
}
