/*
 * A326307  a(n) is the index of n in the ordered list of the numbers with the
 *          same digits as n.
 * https://oeis.org/A326307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = rank of n among all permutations of its digits that do
 *            not * start with zero, sorted in ascending order. The rank is
 *            1‑based. For each * position we count how many smaller digits
 *            can be placed there taking into * account the remaining
 *            multiset. * * The implementation computes the digit mul...
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Argument and return types defined by the build system. */
seq_t a(idx_t n)
{
    /* Helper constants: maximum number of decimal digits for a uint64_t */
    const idx_t MAXLEN = 20;
    idx_t digits_rev[MAXLEN];
    idx_t digits[MAXLEN];
    int   cnt[10];
    idx_t len = 0;
    idx_t tmp = n;

    /* Decompose n into its decimal digits (least significant first). */
    if (tmp == 0) {
        len = 1;
        digits_rev[0] = 0;
    } else {
        while (tmp > 0) {
            digits_rev[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Reverse to obtain most significant first. */
    for (idx_t i = 0; i < len; ++i)
        digits[i] = digits_rev[len - 1 - i];

    /* Count occurrences of each digit. */
    for (int d = 0; d < 10; ++d) cnt[d] = 0;
    for (idx_t i = 0; i < len; ++i)
        ++cnt[digits[i]];

    /* Pre‑compute factorials up to len. */
    seq_t fact[MAXLEN + 1];
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= len; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Compute the lexicographic rank (1‑based). */
    seq_t rank = (seq_t)1;

    for (idx_t pos = 0; pos < len; ++pos) {
        int cur = digits[pos];
        int start = (pos == 0) ? 1 : 0;      /* leading zero not allowed */

        for (int d = start; d < cur; ++d) {
            if (cnt[d] == 0) continue;

            /* Temporarily use digit d at this position. */
            --cnt[d];

            /* Compute number of permutations of the remaining digits. */
            idx_t rem = len - pos - 1;
            seq_t numer = fact[rem];
            seq_t denom = (seq_t)1;
            for (int k = 0; k < 10; ++k)
                denom *= fact[cnt[k]];

            seq_t add = numer / denom;
            rank += add;

            /* Restore count. */
            ++cnt[d];
        }

        /* Commit the current digit. */
        --cnt[cur];
    }

    return rank;
}
