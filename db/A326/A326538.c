/*
 * A326538  a(n) is the numerator of the image of 1/n by the Cantor staircase
 *          function.
 * https://oeis.org/A326538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2434, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   gcd, pow2, step_ternary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A326538: a(n) = numerator of Cantor staircase function applied to 1/n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/step_ternary.h"

/* sequence function */
seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;          /* c(1) = 1 */
    }

    /* allocate arrays */
    idx_t *seen = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    uint8_t *bits = (uint8_t *)malloc((size_t)n * sizeof(uint8_t));
    if (!seen || !bits) {
        return (seq_t)0;          /* memory guard */
    }

    /* initialize remainder tracking */
    for (idx_t i = 0; i < (idx_t)n; ++i) {
        seen[i] = (idx_t)-1;
    }

    idx_t r = (idx_t)1;          /* initial remainder */
    seen[r] = (idx_t)0;          /* seen at position 0 */
    idx_t bits_len = 0;
    bool terminated = false;
    idx_t cycle_start = 0, cycle_len = 0;

    /* Generate ternary digits of 1/n, mapping to binary bits */
    while (1) {
        idx_t next_r;
        uint8_t d;
        step_ternary(&r, n, &d, &next_r);
        r = next_r;

        /* Terminate if digit 1 occurs */
        if (d == (uint8_t)1) {
            bits[bits_len++] = (uint8_t)1;   /* digit 1 maps to 1 in binary */
            terminated = true;
            break;
        } else {
            bits[bits_len++] = (d == (uint8_t)2) ? (uint8_t)1 : (uint8_t)0;
        }

        /* Detect cycle */
        if (seen[r] != (idx_t)-1) {
            cycle_start = seen[r];
            cycle_len = bits_len - cycle_start;
            break;
        } else {
            seen[r] = bits_len;            /* store position of this remainder */
        }
    }

    seq_t result = (seq_t)0;

    if (terminated) {
        /* Finite binary fraction: value = bits / 2^bits_len */
        seq_t num = (seq_t)0;
        for (idx_t i = 0; i < bits_len; ++i) {
            num = (num << 1) | (seq_t)bits[i];
        }
        seq_t den = pow2(bits_len);
        seq_t g = gcd(num, den);
        num /= g;
        result = num;
    } else {
        /* Repeating binary fraction */
        seq_t prefix_val = (seq_t)0;
        for (idx_t i = 0; i < cycle_start; ++i) {
            prefix_val = (prefix_val << 1) | (seq_t)bits[i];
        }
        seq_t period_val = (seq_t)0;
        for (idx_t i = cycle_start; i < bits_len; ++i) {
            period_val = (period_val << 1) | (seq_t)bits[i];
        }

        seq_t powL_minus1 = pow2(cycle_len) - (seq_t)1;
        seq_t num = prefix_val * powL_minus1 + period_val;
        seq_t den = pow2(cycle_start) * powL_minus1;

        seq_t g = gcd(num, den);
        num /= g;
        result = num;
    }

    free(seen);
    free(bits);
    return result;
}
