/*
 * A262039  Nearest palindrome to n; in case of a tie choose the larger
 *          palindrome.
 * https://oeis.org/A262039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nearest palindrome to n (in base 10); ties favor the larger one. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/rev_decimal.h"

/* Construct the palindrome that has the same length as `total_len` and
   whose left half (plus middle digit if odd) equals `prefix`. */
static seq_t make_pal_from_prefix(seq_t prefix, size_t total_len)
{
    size_t k = total_len / 2;
    if (total_len % 2 == 0) {               /* even length */
        seq_t powk = pow10(k);
        seq_t rev = rev_decimal(prefix);    /* reverse the whole prefix */
        return prefix * powk + rev;
    } else {                                 /* odd length */
        seq_t powk   = pow10(k);
        seq_t powk1  = pow10(k + 1);
        seq_t mid    = prefix % 10;
        seq_t left   = prefix / 10;
        seq_t rev    = rev_decimal(left);
        return left * powk1 + mid * powk + rev;
    }
}

/* Returns the nearest palindrome to n in base 10.
   If two palindromes are equally close, returns the larger one. */
seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;                /* convert index to seq_t */
    /* Handle the trivial case n = 0. */
    if (num == 0)
        return 0;

    /* 1. Determine the number of decimal digits of n. */
    size_t len = 0;
    seq_t tmp = num;
    do {
        len++;
        tmp /= 10;
    } while (tmp > 0);

    /* 2. Compute the prefix that contains the left half (and middle if odd). */
    size_t prefix_len = (len + 1) / 2;          /* ceil(len/2) */
    size_t front_len  = len - prefix_len;      /* remaining right side */
    seq_t div_front   = pow10(front_len);       /* 10^(front_len) */
    seq_t prefix_val  = num / div_front;        /* integer division gives the prefix */

    /* 3. Mirror the number itself to obtain the central candidate. */
    seq_t mirror = make_pal_from_prefix(prefix_val, len);

    /* If the number itself is a palindrome, return it. */
    if (mirror == num)
        return num;

    /* 4. Determine the greater-or-equal and less-or-equal candidates. */
    seq_t low_candidate, up_candidate;

    if (mirror < num) {
        /* mirror is below n: use it as the lower candidate */
        low_candidate = mirror;
    } else {
        /* mirror is above n: we need a lower palindrome */
        /* Decrement the prefix */
        seq_t prefix_prev = prefix_val - 1;
        seq_t pow10_prev  = pow10(prefix_len - 1);
        if (prefix_prev < pow10_prev) {
            /* Prefix lost a digit → palindrome becomes all 9's with length len-1 */
            low_candidate = pow10(len - 1) - 1;
        } else {
            low_candidate = make_pal_from_prefix(prefix_prev, len);
        }
    }

    if (mirror > num) {
        /* mirror is above n: use it as the upper candidate */
        up_candidate = mirror;
    } else {
        /* mirror is below n: we need an upper palindrome */
        /* Increment the prefix */
        seq_t prefix_next = prefix_val + 1;
        seq_t pow10_next  = pow10(prefix_len);
        size_t total_len_next;
        if (prefix_next >= pow10_next) {
            /* Prefix gained a digit → length increases by 1 */
            total_len_next = len + 1;
        } else {
            total_len_next = len;
        }
        up_candidate = make_pal_from_prefix(prefix_next, total_len_next);
    }

    /* 5. Choose the closer (or larger in case of tie). */
    seq_t diff_low  = (num > low_candidate) ? (num - low_candidate) : (low_candidate - num);
    seq_t diff_up   = (up_candidate > num) ? (up_candidate - num) : (num - up_candidate);

    return (diff_up <= diff_low) ? up_candidate : low_candidate;
}
