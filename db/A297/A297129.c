/*
 * A297129  Numbers having a down-first zigzag pattern in base 4; see
 *          Comments.
 * https://oeis.org/A297129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*  OEIS A297129
 *  Values are the nth numbers whose base‑4 representation has no equal
 *  adjacent digits and whose first comparison (between the two most
 *  significant digits) is down.  The sequence starts at n = 1 with
 *  a(1) = 4.  The function computes a(n) by constructing the base‑4
 *  digits from most significant to least significant using
 *  combinatorial counting.  It works for any seq_t type supported by
 *  the build (up to unsigned __int128 or arbitrary‑precision). */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;            /* rank within numbers of a given length */
    seq_t pow3[64];                    /* powers of 3, up to 3^(L-2) */
    pow3[0] = 1;                       /* 3^0 */
    int L = 2;                         /* minimal length (2 digits) */
    seq_t p = 1;                       /* p = 3^(L-2) */

    /* Find the length L of the base‑4 representation of a(n) */
    while (1) {
        seq_t count_len = (seq_t)6 * p; /* 6 * 3^(L-2) numbers of length L */
        if (target <= count_len) break;
        target -= count_len;
        L++;                           /* increase length */
        p *= 3;                        /* update p = 3^(L-2) */
        pow3[L-2] = p;                 /* store the new power for later use */
    }

    /* Now pow3[0..L-2] are available, and p == pow3[L-2] */

    /* Choose the most significant digit (must be 1..3) */
    int first_digit = 1;
    for (; first_digit <= 3; ++first_digit) {
        seq_t cnt = (seq_t)first_digit * p;   /* d0 * 3^(L-2) */
        if (target <= cnt) break;
        target -= cnt;
    }

    seq_t value = (seq_t)first_digit;
    int prev = first_digit;
    int remaining = L - 1;          /* digits still to choose */

    /* Choose the second digit (must be less than first_digit) */
    int second_digit = 0;
    for (; second_digit < first_digit; ++second_digit) {
        seq_t cnt = p;              /* 3^(L-2) completions after this choice */
        if (target <= cnt) break;
        target -= cnt;
    }
    value = value * 4 + (seq_t)second_digit;
    prev = second_digit;
    --remaining;                    /* one more digit remains to decide */

    /* Choose the remaining digits (no two adjacent equal) */
    while (remaining > 0) {
        int chosen = 0;
        for (int d = 0; d < 4; ++d) {
            if (d == prev) continue;
            seq_t cnt = pow3[remaining - 1];     /* 3^(remaining-1) ways after this choice */
            if (target <= cnt) {
                chosen = d;
                break;
            }
            target -= cnt;
        }
        value = value * 4 + (seq_t)chosen;
        prev = chosen;
        --remaining;
    }

    return value;
}
