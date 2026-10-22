/*
 * A353116  Base-8 representation of A000422(n).
 * https://oeis.org/A353116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007094(A000422(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   digits_of, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000422(n) is the integer obtained by concatenating n, n-1, …, 1 in decimal. */
/* A007094(x) interprets the base‑8 representation of x as a decimal integer. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digits_of.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Step 1: build A000422(n) in base 10, using only seq_t arithmetic. */
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 safely */

    seq_t val = (seq_t)0;

    for (idx_t k = n; k > 0; --k) {
        /* Append decimal representation of k to val: val = val * 10^digits_of(k) + k */
        idx_t d = digits_of(k);
        for (idx_t i = 0; i < d; ++i) {
            /* multiply by 10 d times */
            val = mul_small(val, 10);
        }
        val += (seq_t)k;
        if (k == 1) break;   /* guard against underflow if n==0 */
    }

    /* Step 2: convert val to base‑8 digits (least significant first). */
    seq_t tmp = val;
    size_t cap = 64;          /* initial capacity for base‑8 digits array */
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits) return (seq_t)0;   /* allocation failure – unlikely */

    while (tmp > 0) {
        if (len >= cap) {
            cap <<= 1;            /* double capacity */
            uint8_t *newp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
            if (!newp) { free(digits); return (seq_t)0; }
            digits = newp;
        }
        /* tmp % 8 is guaranteed to fit in a small integer (0‑7) */
        uint8_t dgt = (uint8_t)(unsigned long long)(tmp % 8);
        digits[len++] = dgt;
        tmp /= (seq_t)8;
    }

    /* Step 3: build the decimal integer that reads the base‑8 digits. */
    seq_t result = (seq_t)0;
    for (size_t idx = len; idx > 0; ) {
        --idx;
        result = mul_small(result, 10);
        result += (seq_t)digits[idx];
    }

    free(digits);
    return result;
}
