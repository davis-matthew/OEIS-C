/*
 * A377341  Decimal expansion of the surface area of a truncated octahedron
 *          with unit edge length.
 * https://oeis.org/A377341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit of 6 + 12*sqrt(3) with offset 2. For n =
 *            2,3 the digits are (2,6). For n ≥ 4, let k = n-3. Then a(n) = (
 *            ⌊ 12*sqrt(3) * 10^k ⌋ ) mod 10 . The function implements the
 *            digit‑by‑digit integer‑square‑root algorithm on the integer N =
 *            432 * 10^(2k) (= (12*sqrt(3))^2 * 10^(2k)) whi...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   add_small, mul_small_to, sub_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/add_small.h"
#include "../utils/mul_small_to.h"
#include "../utils/sub_big.h"

/* Max number of decimal digits that may be needed in intermediate
   calculations.  10 000 digits of the sequence require at most
   2 000 200 decimal digits in the algorithm.  */
#define MAX_DIG 20010

/* Multiply a little‑endian decimal number a (lenA digits) by a small
   integer mult and store the result back into a.  */
static void mul_small_inplace(unsigned char *a, size_t *lenA, unsigned int mult)
{
    unsigned long long carry = 0;
    size_t i;
    for (i = 0; i < *lenA; ++i) {
        unsigned long long prod = (unsigned long long)a[i] * mult + carry;
        a[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        a[*lenA] = carry % 10;
        carry /= 10;
        ++(*lenA);
    }
}

/* Compare two little‑endian decimal numbers a and b.  Returns
   -1 if a<b, 0 if a==b, +1 if a>b.  */
static int cmp(const unsigned char *a, size_t lenA,
               const unsigned char *b, size_t lenB)
{
    if (lenA != lenB) return (lenA < lenB) ? -1 : 1;
    size_t i = lenA;
    while (i--) {
        unsigned char da = a[i];
        unsigned char db = b[i];
        if (da != db) return (da < db) ? -1 : 1;
    }
    return 0;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Handle the first two integer digits explicitly. */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n < 2)  return (seq_t)0;   /* undefined, return 0 */

    /* k = position in the fractional part (n-3).  */
    idx_t k = n - 3;

    /* Array for the growing remainder R and the partial root P.
       Little‑endian decimal digit storage.  */
    unsigned char R[MAX_DIG];
    size_t lenR = 1;
    R[0] = 0;

    unsigned char P[MAX_DIG];
    size_t lenP = 1;
    P[0] = 0;

    /* Length of the decimal representation of N = 432 * 10^(2k). */
    idx_t total_len = 3 + 2 * k;           /* always odd (3 + even)   */
    idx_t first_len = total_len % 2;      /* 1 in this problem        */
    idx_t pos = 0;                        /* position in the digit stream */
    int next_group_size = (int)first_len;

    /* Temporary buffer for candidate digits during the search. */
    unsigned char temp[MAX_DIG];
    size_t lenTemp;

    while (pos < total_len) {
        /* Build the next group value (0 … 99).  */
        int gv = 0;
        /* ----- collect group digits ----- */
        for (int t = 0; t < next_group_size; ++t) {
            int digit;
            if (pos < 3) {
                if (pos == 0) digit = 4;
                else if (pos == 1) digit = 3;
                else                 digit = 2;
            } else {
                digit = 0;
            }
            gv = gv * 10 + digit;
            ++pos;
        }

        /* Update remainder: R = R * 100 + gv.  */
        mul_small_inplace(R, &lenR, 100);
        add_small(R, &lenR, (unsigned int)gv);

        /* Find the largest digit d (0 … 9) such that
           (20*P + d) * d <= R.  */
        unsigned int chosen = 0;
        for (unsigned int d = 9; d <= 9; d--) {   /* unsigned loop guard */
            mul_small_to(P, lenP, 20 * d, temp, &lenTemp);
            add_small(temp, &lenTemp, d * d);
            if (cmp(temp, lenTemp, R, lenR) <= 0) {
                chosen = d;
                break;
            }
            if (d == 0) break;   /* for the unsigned loop */
        }

        /* Update root: P = P * 10 + chosen.  */
        mul_small_inplace(P, &lenP, 10);
        add_small(P, &lenP, chosen);

        /* Update remainder: R = R - temp.  */
        sub_big(R, &lenR, temp, lenTemp);

        /* Subsequent groups have size 2.  */
        next_group_size = 2;
    }

    /* The desired digit is the least‑significant decimal digit of P.  */
    return (seq_t)(unsigned int)P[0];
}
