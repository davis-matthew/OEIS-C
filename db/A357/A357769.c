/*
 * A357769  Positive numbers with decimal expansion d_1, ..., d_w that are
 *          divisible by d_1 + ... + d_k for k = 1..w.
 * https://oeis.org/A357769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer such that for every
   decimal prefix the sum of its digits divides the whole number.
   Computed by brute‑force search. */

#include <stdbool.h>
#include <stddef.h>

/* Helper: test whether a number satisfies the prefix‑sum divisibility
   condition. */
static bool is_valid(seq_t N)
{
    /* Extract decimal digits into a stack (least significant first). */
    uint8_t digits[128];
    size_t len = 0;
    seq_t t = N;
    if (t == (seq_t)0) return false;          /* never reached for positive N */
    while (t > (seq_t)0) {
        digits[len++] = (uint8_t)(t % (seq_t)10);
        t = t / (seq_t)10;
    }

    /* Iterate from most significant digit to least, building prefixes
       and accumulating digit sums, checking divisibility each time. */
    seq_t prefix = (seq_t)0;
    seq_t sum    = (seq_t)0;
    for (size_t i = len; i > 0; --i) {
        uint8_t d = digits[i - 1];
        prefix = prefix * (seq_t)10 + (seq_t)d;
        sum += (seq_t)d;
        if (sum == (seq_t)0) return false;          /* safety check */
        if (N % sum != (seq_t)0) return false;
    }
    return true;
}

/* Public entry point: return the n‑th term of A357769. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n=0, safety return */

    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (true) {
        if (is_valid(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
