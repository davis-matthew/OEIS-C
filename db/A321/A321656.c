/*
 * A321656  Fibonacci numbers with distinct digits.
 * https://oeis.org/A321656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n-th Fibonacci number whose decimal digits are all *
 *            distinct (duplicates in the Fibonacci sequence are skipped)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"

static bool is_distinct_digits(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    for (int i = 0; i < 10; ++i) {
        if (cnt[i] > 1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined offset, return 0 */

    seq_t prev = 0;              /* F0 */
    seq_t curr = 1;              /* F1 */
    idx_t fib_idx = 0;           /* current Fibonacci index */
    idx_t found = 0;             /* how many qualifying terms found */
    seq_t last_val = 0;          /* last stored value (to skip duplicates) */
    bool has_last = false;       /* whether last_val is valid */

    while (found < n) {
        seq_t fib;
        if (fib_idx == 0) {
            fib = 0;
        } else if (fib_idx == 1) {
            fib = 1;
        } else {
            fib = prev + curr;
            prev = curr;
            curr = fib;
        }

        if (is_distinct_digits(fib)) {
            if (!has_last || fib != last_val) {
                ++found;
                if (found == n) return fib;
                has_last = true;
                last_val = fib;
            }
        }

        /* All numbers with more than 10 digits cannot have all distinct digits.
           Once we reach that length we can stop searching. */
        if (digit_len(fib) > 10) break;

        ++fib_idx;
    }

    /* Sequence defined only up to the largest qualifying term. */
    return 0;
}
