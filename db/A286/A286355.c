/*
 * A286355  Beatty sequence of (10 + sqrt(10))/9; complement of A177102.
 * https://oeis.org/A286355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(10 + sqrt(10))/9)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 1545, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9900 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(10)) as floor(sqrt(10*n^2)) */
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * nn;          /* n^2 */
    prod = prod * (seq_t)10;        /* 10*n^2 */
    seq_t s = isqrt_seq(prod);      /* floor(n*sqrt(10)) */

    /* Compute floor((n*10 + floor(n*sqrt(10))) / 9) */
    seq_t sum = nn * (seq_t)10 + s;
    return sum / (seq_t)9;
}
