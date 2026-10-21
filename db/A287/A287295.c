/*
 * A287295  a(2n-1) is concatenation of sequence (1,3,..,2n-3,2n-1,2n-3,..3,1)
 *          and a(2n) is concatenation of sequence
 *          (1,3,..,2n-3,2n-1,2n-1,2n-3,..3,1).
 * https://oeis.org/A287295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   digit_count_uint, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A287295
 *  a(n) is the decimal concatenation of the odd numbers
 *  1,3,…,2k‑1 (k=floor((n+1)/2)) and its reverse, with the middle term
 *  repeated if n is even.  For n odd: [1,3,…,2k‑1,…,3,1]
 *  For n even: [1,3,…,2k‑1,2k‑1,…,3,1]
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_from_str.h"   /* for seq_from_str() */
#include "../utils/digit_count_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return zero */

    bool is_even = (n & 1ULL) == 0;
    idx_t p = is_even ? n / 2 : (n + 1) / 2;  /* p = ceil(n/2) */

    /* compute total length of the digit string */
    size_t sum_forward = 0;
    unsigned int last_d = 0;
    for (idx_t i = 1; i <= p; ++i) {
        unsigned int odd = (unsigned int)((i << 1) - 1);
        unsigned int d = digit_count_uint(odd);
        sum_forward += d;
        last_d = d;                 /* store digits of the middle element */
    }

    size_t total_len = 2 * sum_forward;
    if (!is_even)                    /* odd n: middle appears only once */
        total_len -= last_d;

    char *buf = (char *)malloc(total_len + 1);   /* +1 for NUL */
    if (!buf) return 0;                  /* allocation failure */

    size_t pos = 0;

    /* forward part */
    for (idx_t i = 1; i <= p; ++i) {
        unsigned int odd = (unsigned int)((i << 1) - 1);
        int w = sprintf(buf + pos, "%u", odd);
        pos += (size_t)w;
    }

    /* middle part for even n */
    if (is_even) {
        unsigned int odd = (unsigned int)((p << 1) - 1);
        int w = sprintf(buf + pos, "%u", odd);
        pos += (size_t)w;
    }

    /* reversed part */
    for (idx_t i = p - 1; i >= 1; --i) {
        unsigned int odd = (unsigned int)((i << 1) - 1);
        int w = sprintf(buf + pos, "%u", odd);
        pos += (size_t)w;
    }

    buf[pos] = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
