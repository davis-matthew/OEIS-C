/*
 * A257253  Square array A(row,col) = (1/2) * (A083221(row,col+1) -
 *          A083221(row,col)): half of the first differences of each row of
 *          array constructed from the sieve of Eratosthenes.
 * https://oeis.org/A257253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(row,col) = p_row*(m(col+1)-m(col))/2, where p_row is the
 *            row-th prime and m(k) is the k-th positive integer coprime to
 *            all smaller primes (A083221 row differences / 2); the array is
 *            read by antidiagonals
 * Range:     u8 n <= 54, u16 n <= 2700, u32 n <= 3321, u64 n <= 3321,
 *            u128 n <= 3321, big any n
 * Validated: standard, pass, 82 terms through n = 3180 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n - 1;
    uint64_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= t) d++;
    uint64_t off = t - d * (d + 1) / 2;
    uint64_t r = off + 1;        /* row, 1-based */
    uint64_t c = d + 1 - off;    /* column, 1-based */

    /* first r primes */
    uint64_t pr[400];
    size_t cnt = 0;
    for (uint64_t x = 2; cnt < r && cnt < 400; x++) {
        bool ok = true;
        for (size_t i = 0; i < cnt; i++) {
            if (pr[i] * pr[i] > x) break;
            if (x % pr[i] == 0) { ok = false; break; }
        }
        if (ok) pr[cnt++] = x;
    }
    uint64_t p = pr[r - 1];

    /* find c-th and (c+1)-th integers coprime to the first r-1 primes */
    uint64_t found = 0, mc = 0, mc1 = 0;
    for (uint64_t x = 1; found < c + 1; x++) {
        bool ok = true;
        for (uint64_t i = 0; i + 1 < r; i++) {
            if (x % pr[i] == 0) { ok = false; break; }
        }
        if (ok) {
            found++;
            if (found == c) mc = x;
            else if (found == c + 1) mc1 = x;
        }
    }
    uint64_t g = mc1 - mc;
    return (seq_t)p * (seq_t)g / 2;
}
