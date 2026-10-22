/*
 * A346120  a(n) is the smallest nonnegative number k such that the decimal
 *          expansion of k! contains the string n.
 * https://oeis.org/A346120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1063, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_to_str, multiply_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest k ≥ 0 such that the decimal expansion of k!
 *              contains the decimal string representation of n.
 */

#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/digits_to_str.h"
#include "../utils/multiply_digits.h"

/* 10000! has 35660 decimal digits – 40000 is a comfortable safety margin */
#define MAX_FACT_DIGITS 40000

/* Convert an idx_t number to its decimal representation in a buffer. */
static void idx_to_str(idx_t n, char *buf, size_t bufsize)
{
    snprintf(buf, bufsize, "%ju", (uintmax_t)n);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    char n_str[32];
    idx_to_str(n, n_str, sizeof n_str);

    unsigned char digits[MAX_FACT_DIGITS];
    size_t len = 1;
    digits[0] = 1;                    /* 0! = 1 */

    char fact_buf[MAX_FACT_DIGITS];

    for (idx_t k = 0; ; ++k) {
        digits_to_str(digits, len, fact_buf, sizeof fact_buf);
        if (strstr(fact_buf, n_str) != NULL)
            return (seq_t)k;
        unsigned int mul = (unsigned int)(k + 1);
        multiply_digits(digits, &len, mul);
        if (len > MAX_FACT_DIGITS)
            abort();                  /* should not happen for n ≤ 10000 */
    }
}
