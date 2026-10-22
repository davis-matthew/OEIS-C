/*
 * A343383  Length of the preperiodic part of 'Roll and Subtract' trajectory
 *          of n.
 * https://oeis.org/A343383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(n) = length of the preperiodic part of the “Roll and
 *            Subtract” trajectory of n * (computed with Floyd's cycle‑finding
 *            algorithm)
 * Range:     u8 n <= 1282, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static int digit_len(seq_t x) {
    /* Count decimal digits of x (x >= 0). */
    int len = 0;
    while (x > 0) {
        x = x / 10;
        ++len;
    }
    return (len == 0) ? 1 : len;       /* 0 has one digit */
}

static seq_t roll(seq_t x) {
    if (x < 10) return x;
    int d = digit_len(x);
    /* 10^(d-1) as seq_t */
    seq_t pow = pow10((idx_t)(d - 1));
    seq_t first = x / pow;    /* most significant digit */
    seq_t rest  = x % pow;    /* remaining part */
    return rest * 10 + first;
}

static seq_t f(seq_t x) {
    seq_t r = roll(x);
    return (x > r) ? (x - r) : (r - x);
}

/* ------------------------------------------------------------
 * This function returns the length of the preperiodic part
 * of the Roll-and-Subtract trajectory starting from n.
 * ------------------------------------------------------------ */
seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Step 1: Find first meeting point of tortoise and hare */
    seq_t tortoise = f(start);
    seq_t hare = f(f(start));
    while (tortoise != hare) {
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Step 2: Find start of the cycle (mu) */
    seq_t mu_a = start;
    seq_t mu_b = tortoise;
    idx_t mu_count = 0;
    while (mu_a != mu_b) {
        mu_a = f(mu_a);
        mu_b = f(mu_b);
        ++mu_count;
    }

    return (seq_t)mu_count;
}
