/*
 * A262290  a(0)=0; thereafter, add the smallest nonzero binary number, not a
 *          power of 2, that is not a substring of the binary expansion of
 *          a(n).
 * https://oeis.org/A262290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n+1) = a(n) +
 *            smallest_nonzero_non_power_of_two_not_in_substring_of_binary(a(n))
 * Range:     u8 n <= 44, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Convert a seq_t value to a binary string.  The buffer must be large
   enough (at least 128 characters).  The result is null‑terminated. */
static void seq_to_bin(seq_t x, char *buf) {
    if (x == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char rev[128];
    int pos = 0;
    while (x > 0) {
        rev[pos++] = (x & 1) ? '1' : '0';
        x >>= 1;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = rev[pos - 1 - i];
    buf[pos] = '\0';
}

/* Test whether a seq_t value is a power of two.  We avoid zero. */
static bool is_power_of_two(seq_t x) {
    return x != 0 && ((x & (x - 1)) == 0);
}

/* Find the smallest positive integer that is not a power of two
   and whose binary representation does not occur as a substring
   of the binary representation of 'cur'. */
static seq_t find_next_candidate(seq_t cur) {
    char cur_bin[128];
    seq_to_bin(cur, cur_bin);

    seq_t cand = 3;
    for (;;) {
        if (is_power_of_two(cand)) {
            ++cand;
            continue;
        }
        char cand_bin[128];
        seq_to_bin(cand, cand_bin);
        if (strstr(cur_bin, cand_bin) == NULL)
            return cand;
        ++cand;
    }
}

/* Main sequence function.  Computes a(n) as defined.  It does not
   rely on any global state and can be called in any order. */
seq_t a(idx_t n) {
    seq_t cur = 0;              /* a(0) = 0 */
    if (n == 0)
        return cur;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t add = find_next_candidate(cur);
        cur = cur + add;
    }
    return cur;
}
