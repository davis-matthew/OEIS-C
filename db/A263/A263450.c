/*
 * A263450  Smallest integer k>0 such that there is at least one zero in the
 *          decimal representation of prime(n)^k.
 * https://oeis.org/A263450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k > 0 such that prime(n)^k contains a zero digit
 *            in its decimal representation
 * Range:     u8 none, u16 n <= 2, u32 n <= 32, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digit_frequencies.h"

/* Check whether the decimal representation of x contains a '0' digit. */
static bool has_zero_digit(seq_t x)
{
    unsigned int freq[10];
    digit_frequencies(x, freq);
    return freq[0] > 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */
    uint32_t p = get_prime(n);            /* nth prime (1‑based) */
    seq_t current = (seq_t)p;             /* p^1 */
    for (idx_t k = 1; ; ++k) {
        if (has_zero_digit(current))
            return k;
        current = current * (seq_t)p;     /* p^(k+1) */
    }
}
