/*
 * A261423  Largest palindrome <= n.
 * https://oeis.org/A261423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

/*  Largest palindrome <= n (OEIS A261423)  */
static seq_t reverse_digits(seq_t x, idx_t cnt)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev;
}

/*  Construct the “palindromic floor” P(n) defined in the
    OEIS entry.  This works for any seq_t magnitude.  */
static seq_t palindrome_of_seq(seq_t n)
{
    if (n == 0) return 0;

    /* Count digits of n.  */
    seq_t tmp = n;
    size_t d  = 0;
    while (tmp > 0) {
        tmp = tmp / 10;
        d++;
    }
    size_t half = d / 2;
    seq_t pow_half = pow10(half);          /* 10^half  */
    seq_t left     = n / pow_half;        /* most significant half  */

    seq_t rev;
    if (d % 2 == 0) {
        rev = reverse_digits(left, half);
    } else {
        rev = reverse_digits(left / 10, half);
    }

    return left * pow_half + rev;
}

seq_t a(idx_t n)
{
    if (n < 10)            /* single‑digit numbers are their own palindrome */
        return (seq_t)n;

    seq_t n_seq = (seq_t)n;

    /* P(n) */
    seq_t p = palindrome_of_seq(n_seq);
    if (p <= n_seq)
        return p;

    /* Count digits once more for the power‑of‑10 test. */
    seq_t tmp = n_seq;
    size_t d  = 0;
    while (tmp > 0) {
        tmp = tmp / 10;
        d++;
    }
    seq_t pow_d_minus1 = pow10(d - 1);  /* 10^(d-1) */

    if (n_seq == pow_d_minus1)         /* n is a power of 10  */
        return n_seq - 1;

    size_t half = d / 2;
    seq_t pow_half = pow10(half);      /* 10^half  */
    seq_t m = n_seq - pow_half;        /* n - 10^half  */

    return palindrome_of_seq(m);
}
