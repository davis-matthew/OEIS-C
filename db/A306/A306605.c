/*
 * A306605  Square array T(n, k) read by antidiagonals upwards, n >= 0 and k
 *          >= 0: for any m >= 0, let f_m be the representation of m in the
 *          factorial number system: for any i >= 0, 0 <= f_m(i) <= i and m =
 *          Sum_{i >= 0} f_m(i) * i!; the representation of T(n, k) in the
 *          factorial number system, say g, satisfi...
 * https://oeis.org/A306605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are antidiagonal coordinates of n. *
 *            T(i,j) is obtained by composing the factorial–number–system
 *            digits: * for each position p ≥ 0, the pth digit of T is
 *            f_i(f_j(p)), * i.e. digit of i at position f_j(p). The value is
 *            reconstructed * from its factorial digits by the Horn...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Max number of factorial digits needed for indices up to 10000.
   8! = 40320 > 10000, so 8 digits (positions 0..7) are enough;
   we use 21 for safety and to hold the digits of i as well. */
#define MAX_DIGITS 21

/* Compute factorial digits of m into array digits[0..MAX_DIGITS-1].
   digits[0] is always 0 (by definition of factorial base). */
static void factorial_digits(idx_t m, uint8_t *digits)
{
    digits[0] = 0;
    for (idx_t pos = 1; pos < MAX_DIGITS; pos++) {
        digits[pos] = (uint8_t)(m % (pos + 1));
        m /= (pos + 1);
    }
}

/* Antidiagonal coordinates for index n (offset 0).
   Returns i and j such that the (i,j)-th entry is the n-th term in
   the antidiagonal reading that goes 'upwards' (i decreases). */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2.
       s is the sum i+j. */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    idx_t prev = s * (s + 1) / 2;
    idx_t d = n - prev;      /* position within the current antidiagonal */
    *i = s - d;              /* i decreases along the antidiagonal */
    *j = d;                  /* j increases */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    if (i == 0 || j == 0)
        return (seq_t)0;

    uint8_t digits_i[MAX_DIGITS];
    uint8_t digits_j[MAX_DIGITS];
    factorial_digits(i, digits_i);
    factorial_digits(j, digits_j);

    seq_t result = (seq_t)0;
    /* Reconstruct value from its factorial digits.
       Traverse from the largest possible position down to 0. */
    for (idx_t pos = MAX_DIGITS - 1; pos != (idx_t)-1; --pos) {
        uint8_t d = digits_j[pos];
        uint8_t g = digits_i[d];
        result = result * ((seq_t)(pos + 1)) + (seq_t)g;
    }
    return result;
}
