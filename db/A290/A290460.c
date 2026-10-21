/*
 * A290460  Triangle formed by reading the triangle of Eulerian numbers
 *          (A173018) mod 10.
 * https://oeis.org/A290460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Eulerian(r, c) mod 10, where r and c satisfy * n =
 *            r*(r+1)/2 + c (0 ≤ c ≤ r)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static idx_t triangular_row(idx_t n)
{
    /* Binary search for largest r with r*(r+1)/2 ≤ n */
    idx_t lo = 0;
    idx_t hi = (idx_t)(sqrt((double)n * 2.0) + 2.0);
    if (hi > n) hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= (__int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static uint8_t **euler_rows = NULL;
static idx_t  euler_rows_cnt = 0;   /* number of rows already computed */
static idx_t  euler_rows_cap = 0;   /* capacity of euler_rows array */

static void ensure_rows(idx_t row)
{
    if (row < euler_rows_cnt) return;

    if (euler_rows_cap <= row) {
        idx_t new_cap = euler_rows_cap ? euler_rows_cap << 1 : 4;
        while (new_cap <= row) new_cap <<= 1;
        euler_rows = (uint8_t **)realloc(euler_rows, new_cap * sizeof(uint8_t *));
        euler_rows_cap = new_cap;
    }

    for (idx_t r = euler_rows_cnt; r <= row; ++r) {
        uint8_t *curr = (uint8_t *)malloc((r + 1) * sizeof(uint8_t));
        if (r == 0) {
            curr[0] = 1;
        } else {
            uint8_t *prev = euler_rows[r - 1];
            for (idx_t k = 0; k <= r; ++k) {
                int v = 0;
                if (k <= r - 1) {
                    v += ((k + 1) % 10) * (int)prev[k];
                }
                if (k >= 1) {
                    v += ((r - k) % 10) * (int)prev[k - 1];
                }
                curr[k] = v % 10;
            }
        }
        euler_rows[r] = curr;
    }
    euler_rows_cnt = row + 1;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    __int128 tri128 = (__int128)r * (r + 1) / 2;
    idx_t tri = (idx_t)tri128;
    idx_t c = n - tri;

    ensure_rows(r);
    uint8_t val = euler_rows[r][c];
    return (seq_t)val;
}
