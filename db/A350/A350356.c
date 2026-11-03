/*
 * A350356  Numbers k such that the k-th composition in standard order is
 *          down/up.
 * https://oeis.org/A350356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for the nth down/up composition index. */

#include <stdbool.h>

/* Test whether the composition corresponding to k is down/up. */
static bool is_down_up(idx_t k)
{
    if (k == 0) return true;           /* empty composition */

    unsigned int pos[64];
    int len = 0;
    idx_t t = k;

    /* collect positions of 1 bits (1‑based) in increasing order */
    while (t) {
        unsigned int i = __builtin_ctzll(t);  /* index of lowest set bit */
        pos[len++] = i + 1;
        t &= t - 1;                          /* remove lowest set bit */
    }

    if (len <= 1) return true;  /* single part or empty always down/up */

    /* first differences of the position sequence */
    unsigned int d[64];
    d[0] = pos[0];
    for (int i = 1; i < len; ++i)
        d[i] = pos[i] - pos[i - 1];

    /* test alternating inequalities on the reversed difference array */
    unsigned int rev_prev = d[len - 1];
    int rev_j = 0;                     /* index in the reversed sequence */
    for (int p = len - 2; p >= 0; --p) {
        unsigned int rev_curr = d[p];
        if (rev_j % 2 == 0) {           /* need > for even j  */
            if (!(rev_prev > rev_curr)) return false;
        } else {                        /* need < for odd j   */
            if (!(rev_prev < rev_curr)) return false;
        }
        rev_prev = rev_curr;
        ++rev_j;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0, but safe */
    idx_t target = n;
    idx_t count  = 0;
    idx_t k      = 0;

    while (1) {
        if (is_down_up(k)) {
            ++count;
            if (count == target)
                return (seq_t)k;
        }
        ++k;
    }
}
