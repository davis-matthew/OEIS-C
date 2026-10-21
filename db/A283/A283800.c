/*
 * A283800  Numbers such that the sum of trits of its balanced ternary
 *          representation is 1 or -1.
 * https://oeis.org/A283800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose balanced ternary digit sum is ±1. */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff the balanced‑ternary digit sum of x is +1 or -1. */
static bool is_good(seq_t x)
{
    long long sum = 0;            /* Sum of digits. */
    while (x > 0) {
        seq_t r = x % (seq_t)3;   /* Remainder in {0,1,2}. */
        x /= (seq_t)3;
        if (r == (seq_t)2) {
            sum -= 1;             /* Digit -1. */
            x += (seq_t)1;        /* Carry. */
        } else {
            sum += (long long)r;  /* r is 0 or 1. */
        }
    }
    return sum == 1 || sum == -1;
}

/* External function definition: a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined case, return 0. */
    idx_t   count     = 0;
    seq_t   candidate = 1;

    while (count < n) {
        if (is_good(candidate)) {
            ++count;
        }
        ++candidate;
    }
    /* candidate was incremented after the last accepted term; subtract one. */
    return candidate - (seq_t)1;
}
