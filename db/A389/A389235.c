/*
 * A389235  Number of repeating Harshad algorithm steps until a non-Harshad
 *          number or a power of 10 is reached.
 * https://oeis.org/A389235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of Harshad algorithm steps until a non‑Harshad
 *            number or a power of 10 is reached
 * Range:     u8 n <= 2269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* digit_sum: returns sum of decimal digits of v */
static seq_t digit_sum_local(seq_t v) {
    seq_t sum = 0;
    while (v > 0) {
        sum += v % 10;
        v   /= 10;
    }
    return sum;
}

/* is_power_of_10: true if v == 1 or v consists only of trailing zeros
   in decimal representation (i.e. v = 10^k). */
static bool is_power_of_10(seq_t v) {
    if (v == 1) return true;          /* 1 is treated as 10^0 */
    while (v % 10 == 0) {
        v /= 10;
    }
    return v == 1;
}

/* a(n): Number of Harshad algorithm steps starting from n until a
   non‑Harshad number or a power of 10 is reached. */
seq_t a(idx_t n) {
    seq_t num  = (seq_t)n;          /* copy into seq_t type */
    if (is_power_of_10(num)) return 0;

    seq_t steps = 0;
    while (1) {
        seq_t s = digit_sum_local(num);
        if (num % s != 0) break;          /* non‑Harshad, stop */
        steps++;
        num = num / s;                    /* next number */
        if (is_power_of_10(num)) break;   /* reached a power of 10 */
    }
    return steps;
}
