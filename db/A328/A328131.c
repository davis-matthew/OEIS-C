/*
 * A328131  Erase all zero digits from decimal expansion of n. Then keep just
 *          the coprime digits; write 0 if all digits disappear.
 * https://oeis.org/A328131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = keep digits of n that are non‑zero and coprime with all *
 *            other non‑zero digits in the decimal expansion; if no such
 *            digits exist, * return 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* int, size_t if needed */

/* small gcd for values 0‑9 (or small digits) */
static unsigned gcd_small(unsigned a, unsigned b) {
    while (b) {
        unsigned t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, but guard */
    /* maximum decimal digits of a 64‑bit unsigned integer is 20 */
    unsigned digits[25];
    bool is_nonzero[25];
    int len = 0;
    idx_t t = n;
    while (t > 0 && len < 25) {
        unsigned d = t % 10;
        digits[len] = d;
        is_nonzero[len] = (d != 0);
        t /= 10;
        len++;
    }

    /* indices of non‑zero digits (stored in reverse order) */
    int nz_idx[25];
    int nz_len = 0;
    for (int i = 0; i < len; ++i) {
        if (is_nonzero[i]) nz_idx[nz_len++] = i;
    }

    /* decide which digits to keep */
    bool keep[25] = {false};
    for (int ii = 0; ii < nz_len; ++ii) {
        unsigned d = digits[nz_idx[ii]];
        bool ok = true;
        for (int jj = 0; jj < nz_len; ++jj) {
            if (ii == jj) continue;
            unsigned other = digits[nz_idx[jj]];
            if (gcd_small(d, other) != 1) {
                ok = false;
                break;
            }
        }
        keep[nz_idx[ii]] = ok;
    }

    /* rebuild the result in original order (most significant first) */
    seq_t result = 0;
    bool any = false;
    for (int pos = len - 1; pos >= 0; --pos) {
        if (keep[pos]) {
            result = result * (seq_t)10 + (seq_t)digits[pos];
            any = true;
        }
    }
    return any ? result : (seq_t)0;
}
