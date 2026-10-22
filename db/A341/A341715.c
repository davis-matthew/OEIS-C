/*
 * A341715  a(n) = smallest prime of the form n||n+1||n+2||...||n+k, where ||
 *          denotes decimal concatenation, or -1 if no such prime exists.
 * https://oeis.org/A341715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, more, nice
 * Formula:   a(n) = smallest prime of the form n||n+1||…||n+k, or -1 if no
 *            such prime exists (offset 2)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   concat_parts, digits, is_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_prime.h"

/* Helper: concatenate a (seq_t) with an idx_t b.
   Returns a * 10^len(b) + b. */
static seq_t concat_seq_idx(seq_t a, idx_t b)
{
    /* digits(b) returns number of decimal digits of b. */
    idx_t d = (idx_t)digits(b);
    seq_t pow = pow10(d);
    return a * pow + b;
}

seq_t a(idx_t n)
{
    /* a(2) is the first term; values for n < 2 are undefined */
    if (n < 2) {
        /* No defined value below offset; return -1 sentinel. */
        return (seq_t)(-1);
    }

    /* If n itself is prime, return n. */
    if (is_prime(n)) {
        return (seq_t)n;
    }

    /* Start concatenating from n and n+1. */
    seq_t cur = concat_parts(n, n + 1);
    idx_t k = 1;          /* we have included n+1 so far */

    while (1) {
        /* Test whether the current concatenated number is prime. */
        if (is_prime_big(cur)) {
            return cur;
        }

        /* Append next integer. */
        ++k;
        idx_t next = n + k;   /* next integer to append */
        cur = concat_seq_idx(cur, next);
    }

    /* Unreachable, but required to avoid compiler warnings. */
    return (seq_t)(-1);
}
