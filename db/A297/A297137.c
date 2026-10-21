/*
 * A297137  Numbers having an up-first zigzag pattern in base 7; see Comments.
 * https://oeis.org/A297137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n-th positive integer whose base‑7 representation
 *            has successive digits all different and whose first comparison
 *            (from the most significant to the next) is an up (i.e. second
 *            digit > first digit)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the number directly
   without listing all preceding terms. */

#include <stdbool.h>
#include <stdint.h>

/* pre‑computed powers of 6 (used for counting suffixes) */
static idx_t _pow6[64];
static bool _pow6_ready = false;

static void init_pow6(void)
{
    if (_pow6_ready) return;
    _pow6[0] = (idx_t)1;
    for (int i = 1; i < 64; ++i)
        _pow6[i] = _pow6[i - 1] * (idx_t)6;
    _pow6_ready = true;
}

/* compute the n-th term of A297137 (n >= 1) */
seq_t a(idx_t n)
{
    init_pow6();
    if (n < 1) return (seq_t)0;          /* safety for out‑of‑range */

    /* find the length L of the desired number         */
    /* minimum length is 2 (base‑7 numbers start at 12) */
    idx_t L = 2;
    idx_t total_before = 0;              /* count of numbers with length < L */

    while (true) {
        /* count of L‑digit numbers:
           sum_{first=1..6} (6 - first) * 6^{L-2}
           = 15 * 6^{L-2}                                 */
        idx_t count_len = (idx_t)15 * _pow6[L - 2];
        if (n <= total_before + count_len) break;
        total_before += count_len;
        ++L;
    }

    /* local index (1‑based) within the block of L‑digit numbers */
    idx_t k = n - total_before;
    seq_t   value = 0;     /* will build the number in base‑7   */
    int     prev_digit = -1;  /* no previous digit yet           */
    idx_t   rem = L;          /* digits remaining to place       */

    /* first digit (must be 1..6)                                 */
    for (int first = 1; first <= 6; ++first) {
        idx_t cnt_first = (6 - first) * _pow6[L - 2];
        if (k > cnt_first) {          /* skip whole block           */
            k -= cnt_first;
            continue;
        }
        value = (seq_t)first;
        prev_digit = first;
        rem = L - 1;                  /* remaining digits to place  */
        break;
    }

    /* second digit (must be > first)                               */
    for (int second = prev_digit + 1; second <= 6; ++second) {
        idx_t cnt_second = _pow6[L - 2];
        if (k > cnt_second) {
            k -= cnt_second;
            continue;
        }
        value = value * (seq_t)7 + (seq_t)second;
        prev_digit = second;
        rem = L - 2;
        break;
    }

    /* remaining digits: at each step any digit except the previous */
    while (rem > 0) {
        for (int nd = 0; nd <= 6; ++nd) {
            if (nd == prev_digit) continue;
            idx_t cnt = _pow6[rem - 1];   /* number of completions   */
            if (k > cnt) {
                k -= cnt;
                continue;
            }
            value = value * (seq_t)7 + (seq_t)nd;
            prev_digit = nd;
            --rem;
            break;
        }
    }

    return value;
}
