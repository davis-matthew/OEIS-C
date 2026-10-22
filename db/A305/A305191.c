/*
 * A305191  Table read by rows: T(n,k) is the number of pairs (x,y) mod n such
 *          that x^2 + y^2 == k (mod n), for k from 0 to n-1.
 * https://oeis.org/A305191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n
 *            and k = n - r(r-1)/2 - 1. T(r,k) counts the number of pairs
 *            (x,y) modulo r such that x² + y² ≡ k (mod r). The implementation
 *            below simply enumerates all residue pairs; for the intended
 *            range (n ≤ 10 000) this is easily fast enough
 * Range:     u8 n <= 1775, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – not used for valid calls */

    /* ---------- find the row r and column k ----------------- */
    uint64_t t = (uint64_t)n;          /* indices fit in 64‑bit integer */
    uint64_t r = (uint64_t)((sqrt((long double)8.0L * t - 7.0L) - 1.0L) / 2.0L);
    /* adjust in case of rounding error */
    while (r * (r + 1) / 2 < t) ++r;

    uint64_t prev = (r - 1) * r / 2;   /* total entries before row r */
    uint64_t k    = t - prev - 1;      /* column (0‑based) in row r */

    /* ---------- count solutions to x² + y² ≡ k (mod r) ---------- */
    seq_t count = 0;
    for (uint64_t x = 0; x < r; ++x) {
        uint64_t xsq = (x * x) % r;
        for (uint64_t y = 0; y < r; ++y) {
            uint64_t suma = (xsq + (y * y) % r) % r;
            if (suma == k) {
                ++count;
            }
        }
    }

    return count;
}
