/*
 * A353624  a(0) = 0, and for n > 0, a(n) is the least multiple of n that can
 *          be added to n without carries in balanced ternary.
 * https://oeis.org/A353624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: find the least multiple m = n*k (k≥1) such that the
   balanced‑ternary representation of n and m have no equal non‑zero
   digits in the same position.  This m is returned as a(idx_t n).  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static void balanced_digits(seq_t x, int8_t *buf, size_t *len)
{
    if (x == (seq_t)0) {
        *len = 0;
        return;
    }
    *len = 0;
    while (x > (seq_t)0) {
        int r = (int)(x % (seq_t)3);
        x = x / (seq_t)3;
        if (r == 2) {          /* 2 → -1 with a carry */
            r = -1;
            x = x + (seq_t)1;
        }
        buf[(*len)++] = (int8_t)r;
    }
}

static bool conflict(const int8_t *a, size_t na,
                     const int8_t *b, size_t nb)
{
    size_t i = 0;
    while (i < na || i < nb) {
        int8_t da = (i < na) ? a[i] : 0;
        int8_t db = (i < nb) ? b[i] : 0;
        if (da && db && da == db)
            return true;        /* same non‑zero digit → carry */
        ++i;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sn = (seq_t)n;

    int8_t dn[64];
    size_t len_n;
    balanced_digits(sn, dn, &len_n);

    for (idx_t k = 1;; ++k) {
        seq_t m = sn * (seq_t)k;

        int8_t dm[64];
        size_t len_m;
        balanced_digits(m, dm, &len_m);

        if (!conflict(dn, len_n, dm, len_m))
            return m;
    }
}
