/*
 * A297249  Numbers whose base-3 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑3 down‑variation exceeds its
 *            up‑variation. The function computes this by brute‑force testing
 *            of successive integers
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9117 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a base‑3 number has larger down‑variation than up‑variation. */
static bool sat(seq_t x)
{
    int dv = 0;           /* sum of increases (down‑variation) */
    int uv = 0;           /* sum of decreases (up‑variation) */
    int prev = -1;        /* previous digit (least significant first) */
    seq_t t = x;

    while (t > 0) {
        int d = (int)(t % 3);   /* current base‑3 digit */
        if (prev != -1) {
            if (d > prev)
                dv += d - prev;
            else if (d < prev)
                uv += prev - d;
        }
        prev = d;
        t /= 3;
    }
    return dv > uv;
}

/* Return the n­th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0 */

    idx_t count = 0;
    idx_t cand = 1;                      /* candidate integer to test */

    while (1) {
        seq_t val = (seq_t)cand;
        if (sat(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++cand;
    }
}
