/*
 * A381224  a(n) is the integer resulting from the concatenation of the unit
 *          digit of prime(n-1) to the digits of prime(n) without its own unit
 *          digit.
 * https://oeis.org/A381224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = concatenation of the unit digit of prime(n-1) with the
 *            digits of prime(n) without its own unit digit. a(1) = 0
 * Range:     u8 n <= 25, u16 n <= 1230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    /* prime indices are zero‑based for get_prime, but the sequence
       definition uses 1‑based primes: prime(1)=2, prime(2)=3, ... */
    uint32_t prev_prime = get_prime(n - 1);
    uint32_t curr_prime = get_prime(n);

    /* digit to prepend */
    seq_t d = (seq_t)(prev_prime % 10u);

    /* If the current prime has only one digit, the result is simply the
       preceding digit. */
    if (curr_prime < 10) {
        return d;
    }

    seq_t after = (seq_t)(curr_prime / 10);
    int len     = digit_len(after);    /* number of decimal digits of after */
    seq_t pow   = pow10(len);          /* 10^len */
    return d * pow + after;
}
