/*
 * A359697  Triangle T(n,k), n >= 1, 1 <= k <= n, read by rows, where T(n,k)
 *          is carryless product n X k base 10.
 * https://oeis.org/A359697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base, look
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Validated: standard, pass, 94 terms through n = 4641 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A359697: Triangle T(n,k) with carryless product n X k base 10,
 * read by rows.  For n≥1, let r be the row number (smallest with r(r+1)/2≥n),
 * let k = n – r(r‑1)/2 be the column (1‑based).  Then
 *     a(n) = carryless_product(r, k)
 *
 * The implementation uses only integer arithmetic on idx_t (uint64_t) and
 * seq_t, so it works for any unsigned integer type (including arbitrary-
 * precision) provided by the build.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
static inline seq_t carryless_prod(idx_t a, idx_t b)
{
    /* Extract decimal digits of a and b.  */
    uint8_t da[64] = {0};
    uint8_t db[64] = {0};
    size_t na = 0, nb = 0;

    /* Since a,b ≥ 1 for the triangle, the loops always execute once. */
    do { da[na++] = (uint8_t)(a % 10); a /= 10; } while (a);
    do { db[nb++] = (uint8_t)(b % 10); b /= 10; } while (b);

    /* Result can have at most na + nb - 1 digits.  */
    uint8_t res[128] = {0};
    for (size_t i = 0; i < na; ++i) {
        for (size_t j = 0; j < nb; ++j) {
            uint8_t prod = da[i] * db[j];
            res[i + j] += prod;
            res[i + j] %= 10;          /* carryless addition */
        }
    }

    /* Convert digit array back to seq_t.  */
    seq_t value  = 0;
    seq_t mult   = 1;
    size_t len = na + nb - 1;
    for (size_t i = 0; i < len; ++i) {
        value += ((seq_t)res[i]) * mult;
        mult *= 10;
    }
    return value;
}

/* ------------------------------------------------------------------ */
/*  Find the row number r such that r*(r+1)/2 ≥ n  (n ≥ 1)               */
/* ------------------------------------------------------------------ */
static inline idx_t find_row(idx_t n)
{
    /* Increase upper bound until it satisfies the condition.  */
    idx_t lo = 1, hi = 2;
    while (true) {
        unsigned __int128 val = (unsigned __int128)hi * (hi + 1) / 2;
        if (val >= n) {
            break;
        }
        hi <<= 1;
    }
    /* Binary search between lo and hi.  */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

/* ------------------------------------------------------------------ */
/*  Carryless product of two unsigned integers in base 10.               */
/*  The digits are processed least significant first; each summation is
 *  performed modulo 10 (carryless addition).  Multiplication uses
 *  the usual integer multiplication (not modulo).                     */

/* ------------------------------------------------------------------ */
/*  Public entry point required by the build system.                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* defined only for n≥1, return 0 otherwise */
    }
    idx_t r = find_row(n);
    idx_t t_prev = (r - 1) * r / 2;   /* number of elements before row r */
    idx_t k = n - t_prev;            /* column index (1‑based) */
    return carryless_prod(r, k);
}
