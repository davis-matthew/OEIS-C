/*
 * A359496  Nonnegative integers whose sum of positions of 1's in their binary
 *          expansion is less than the sum of positions of 1's in their
 *          reversed binary expansion, where positions in a sequence are read
 *          starting with 1 from the left.
 * https://oeis.org/A359496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth nonnegative integer x (starting with 1) such
 *            that if the binary expansion of x is read from left to right
 *            with positions starting at 1, the sum of the positions of the
 *            ‘1’ bits is less than the corresponding sum for the reversed
 *            binary string
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the number of bits in a seq_t value (bit length). */
static inline idx_t bitlen_seq_t(seq_t x) {
    idx_t len = 0;
    while (x != 0) {
        len++;
        x >>= 1;
    }
    return len;
}

/* Test whether a value satisfies the inequality. */
static bool satisfies(seq_t x) {
    if (x == 0)
        return false;                     /* 0 has no 1-bits */
    idx_t L = bitlen_seq_t(x);           /* length of binary representation */
    idx_t C = 0;                          /* count of 1-bits */
    idx_t S_right = 0;                    /* sum of right positions (1‑based) */
    idx_t pos = 1;
    seq_t tmp = x;
    while (tmp != 0) {
        if ((tmp & 1) != 0) {
            ++C;
            S_right += pos;
        }
        ++pos;
        tmp >>= 1;
    }
    /* Condition: 2*S_right > C*(L+1)  <=>  sum_left < sum_reversed */
    return (2 * S_right) > (C * (L + 1));
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* safety net – sequence is defined for n≥1 */

    static seq_t *cache = NULL;   /* stored terms */
    static size_t cap = 0;        /* cache capacity */
    static size_t len = 0;        /* number of terms stored */
    static seq_t candidate = 1;   /* next integer to test */

    while (len < n) {
        if (satisfies(candidate)) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
