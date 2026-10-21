/*
 * A256460  Decimal expansion of 1/273.16, the fraction of the triple point
 *          temperature of water corresponding to 1 K (kelvin).
 * https://oeis.org/A256460
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the (n+4)-th decimal digit of the fraction 25/6829 (i.e.
 *            of 1 / 273.16)
 * Range:     u8 none, u16 n <= 9, u32 n <= 102, u64 n <= 102, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses long-term storage of generated digits.  The
   first time a(n) is evaluated it calculates the decimal expansion
   of 25/6829 via long division, stores each digit in a static array,
   and returns the required one.
   The mapping n → digit index is n+4 because a(-2) corresponds to
   the third digit after the decimal point.  */
#include <stddef.h>

#define MAX_DIGITS 20010      /* enough for n up to about 10000 */

static seq_t digits[MAX_DIGITS];  /* digits[0] is the first decimal digit after '.' */
static size_t digits_len = 0;     /* number of digits already computed */
static seq_t rem = 25;            /* current remainder in the long‑division */
static const seq_t denom = 6829;  /* denominator of the fraction 25/6829 */

seq_t a(idx_t n)
{
    /* calls below the offset are undefined – we simply return 0. */
    if (n < -2) return (seq_t)0;

    /* The digit we need in the expansion of 25/6829.  For n = -2 the
       required digit is the 3rd one after the decimal point, hence the
       offset of 4. */
    size_t target = (size_t)(n + 4);

    /* Generate digits until we have at least target+1 digits. */
    while (digits_len <= target && digits_len < (size_t)MAX_DIGITS) {
        /* Bring down the next decimal place (multiply remainder by 10). */
        seq_t temp   = rem * (seq_t)10;
        seq_t digit  = temp / denom;
        rem          = temp % denom;
        digits[digits_len++] = digit;
    }

    /* If the requested digit is beyond our pre‑computed range, return 0. */
    if (target < digits_len)
        return digits[target];
    return (seq_t)0;
}
