/*
 * A293500  Number of orientable strings of length n using a maximum of k
 *          colors, array read by descending antidiagonals, T(n,k) for n >= 1
 *          and k >= 1.
 * https://oeis.org/A293500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(pos) = T(row,k) where T(n,k) = (k^n - k^ceil(n/2))/2 (the
 *            sequence is read by descending antidiagonals of the infinite
 *            array T(n,k) for n ≥ 1, k ≥ 1)
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 164, u64 n <= 424,
 *            u128 n <= 1113, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* fast power using unsigned __int128 (valid when seq_t is ≤8 bytes) */
static unsigned __int128 pow128(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t pos)
{
    /* locate the diagonal sum s so that (s-1)*s/2 >= pos */
    idx_t low = 2, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < pos) {
        high <<= 1;
        if (high == 0) { /* overflow guard, should not happen for normal inputs */
            high = (idx_t)-1;
            break;
        }
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cnt = (unsigned __int128)mid * (mid - 1) / 2;
        if (cnt >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                                  /* diagonal sum */
    unsigned __int128 prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t offset = (idx_t)((unsigned __int128)pos - prev - 1);  /* 0‑based in diagonal */

    idx_t row = offset + 1;   /* n (exponent) */
    idx_t col = s - row;      /* k (base) */

    idx_t half = (row + 1) >> 1; /* ceil(row/2) */

    if (sizeof(seq_t) <= 8) {        /* integral seq_t: use 128‑bit intermediates */
        unsigned __int128 pow_n  = pow128((unsigned __int128)col, row);
        unsigned __int128 pow_h  = pow128((unsigned __int128)col, half);
        unsigned __int128 diff = pow_n - pow_h;
        unsigned __int128 result = diff / 2;
        return (seq_t)result;
    } else {                         /* arbitrary‑precision seq_t: use helper */
        seq_t pow_n = big_pow((seq_t)col, (seq_t)row);
        seq_t pow_h = big_pow((seq_t)col, (seq_t)half);
        seq_t diff  = pow_n - pow_h;
        return diff / (seq_t)2;
    }
}
