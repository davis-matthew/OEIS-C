/*
 * A329147  Replace in n each nonzero digit d with prime(d).
 * https://oeis.org/A329147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 454, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Replace each nonzero digit d of n with the d‑th prime number.
   The digit 0 is kept unchanged.  For example,
   a(258) = 31119, a(307) = 5017. */
#include <stddef.h>
#include <stdint.h>

static const seq_t prime_of_digit[10] = {
    0,  /* 0 stays 0 */
    2, 3, 5, 7,
    11, 13, 17, 19, 23
};

seq_t a(idx_t n)
{
    /* Special case for 0: the empty digit sequence yields 0. */
    if (n == 0u)
        return (seq_t)0;

    /* Extract digits of n into a stack (reverse order). */
    uint8_t rev[20];               /* uint64_t has at most 20 decimal digits */
    size_t cnt = 0;
    idx_t tmp = n;
    while (tmp > 0u) {
        rev[cnt++] = (uint8_t)(tmp % 10u);
        tmp /= 10u;
    }

    seq_t result = (seq_t)0;
    /* Process digits from most significant to least. */
    for (int i = (int)cnt - 1; i >= 0; --i) {
        uint8_t d = rev[i];
        if (d == 0u) {
            result = result * (seq_t)10u;  /* append a zero */
        } else if (d <= 4u) {
            result = result * (seq_t)10u + prime_of_digit[d];
        } else {
            /* primes 11,13,17,19,23 are two‑digit numbers */
            result = result * (seq_t)100u + prime_of_digit[d];
        }
    }
    return result;
}
