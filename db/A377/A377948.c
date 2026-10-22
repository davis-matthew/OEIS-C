/*
 * A377948  Numbers that have at least 1 repeated decimal digit and whose
 *          decimal digits are nondecreasing as place value decreases.
 * https://oeis.org/A377948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1564, u32 n <= 8837, u64 n <= 8837,
 *            u128 n <= 8837, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers with nondecreasing digits and at least one repeated digit. */
/* (OEIS A377948 – offset 1) Efficient combinatorial generation. */

#include <stdbool.h>
#include <stdint.h>

/* Binomial coefficient for small integers (n <= 50). Results fit in seq_t. */
static seq_t binom(int n, int k)
{
    if (k < 0 || k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t r = 1;
    for (int i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* a(n) : nth number (1‑based) whose decimal digits are nondecreasing and
   contain at least one repeated digit. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* Not used – offset is 1 */

    seq_t n_val = (seq_t)n;           /* promoted for arithmetic */

    /* ---- find the length of the target number ---- */
    int len = 1;                     /* will end up at least 2 */
    seq_t cumulative = 0;            /* total count of valid numbers with < len digits */

    while (1) {
        ++len;
        seq_t valid_len;
        if (len <= 9) {
            /* total nondecreasing sequences of length len */
            seq_t total = binom(len + 8, 8);
            /* sequences with all distinct digits (must skip those) */
            seq_t distinct = binom(9, len);
            valid_len = total - distinct;
        } else {
            /* length > 9 guarantees a repeat by pigeonhole principle */
            valid_len = binom(len + 8, 8);
        }

        if (cumulative + valid_len >= n_val)
            break;

        cumulative += valid_len;
    }

    /* ---- index within numbers of the found length (0‑based) ---- */
    seq_t diff = n_val - cumulative - 1;
    idx_t idx = (idx_t)diff;          /* safe: n <= 10000 -> diff fits in 64 bits */

    /* ---- generate the digits in lexicographic (numerical) order ---- */
    int d[32];                        /* enough for any len up to 32 */
    int prev_digit = 1;               /* placeholder for first digit */
    bool has_dup = false;

    for (int pos = 1; pos <= len; ++pos) {
        int min_digit = (pos == 1) ? 1 : prev_digit;
        for (int digit = min_digit; digit <= 9; ++digit) {
            bool dup_now = has_dup || (pos > 1 && digit == prev_digit);
            int rem = len - pos;       /* remaining positions after this digit */

            seq_t count;
            if (rem == 0) {
                count = dup_now ? 1 : 0;
            } else if (dup_now) {
                /* all sequences of length rem with digits >= digit */
                count = binom(rem + (9 - digit), rem);
            } else {
                /* sequences with at least one repeat among remaining digits */
                seq_t total = binom(rem + (9 - digit), rem);
                seq_t distinct = 0;
                if (9 - digit >= rem)
                    distinct = binom(9 - digit, rem);
                count = total - distinct;
            }

            if ((idx_t)count > idx) {
                d[pos - 1] = digit;
                prev_digit = digit;
                has_dup = dup_now;
                break;
            } else {
                idx -= (idx_t)count;
            }
        }
    }

    /* ---- construct the integer value from its digits ---- */
    seq_t value = 0;
    for (int i = 0; i < len; ++i)
        value = value * 10 + (seq_t)d[i];

    return value;
}
