/*
 * A382137  Smallest integer that cannot be converted to a multiple of n by
 *          changing at most one of its decimal digit.
 * https://oeis.org/A382137
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer m ≥ 11 such that for every decimal
 *            position and for every digit change (at most one change)
 *            resulting number is NOT a multiple of n. The function computes
 *            this by brute‑force search. For each candidate m it tests all
 *            numbers reachable by changing at most one digit; if none...
 * Range:     u8 none, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute number of decimal digits of x (x > 0). */
static int digit_count(seq_t x)
{
    int cnt = 0;
    while (x > 0) {
        x /= 10;
        cnt++;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 11.  For n < 11 we
       return 0 as a placeholder (unused by the tests). */
    if (n < 11)
        return 0;

    /* Cast n to seq_t for modulo operations. */
    seq_t sn = (seq_t)n;

    seq_t m = 11;          /* start from the smallest admissible integer */
    while (true) {
        /* If m itself is a multiple of n, it can be converted by
           changing zero digits, so skip it. */
        if ((m % sn) == 0) {
            m++;
            continue;
        }

        /* Compute length of m and pre‑compute powers of 10 (ascending). */
        int len = digit_count(m);
        seq_t pow10[20];   /* enough for 64‑bit numbers */
        pow10[0] = 1;
        for (int i = 1; i < len; i++)
            pow10[i] = pow10[i - 1] * 10;

        bool bad = false;  /* true if some one‑digit change yields a multiple */

        /* Iterate over each digit position from most significant to least. */
        for (int pos = 0; pos < len; pos++) {
            seq_t pow_pos = pow10[len - 1 - pos];          /* 10^(len-1-pos) */
            int curr_digit = (int)((m / pow_pos) % 10);    /* current digit */

            /* Try replacing it by all other digits 0–9. */
            for (int d = 0; d <= 9; d++) {
                if (d == curr_digit)
                    continue;          /* no change -> already checked */
                seq_t new_val = m + (seq_t)((d - curr_digit) * pow_pos);
                if ((new_val % sn) == 0) {
                    bad = true;
                    break;
                }
            }
            if (bad)
                break;
        }

        if (!bad)
            return m;   /* found the smallest suitable integer */

        m++;           /* test next integer */
    }
}
