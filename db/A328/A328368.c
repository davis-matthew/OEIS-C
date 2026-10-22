/*
 * A328368  Irregular triangle read by rows: T(n,k) is the total number of
 *          parts in all partitions of all positive integers <= n into k
 *          consecutive parts.
 * https://oeis.org/A328368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle entry: a(n) is the n‑th term of the irregular triangle read by rows.
 * The triangle T(r,k) is defined for integers r≥1, k≥1 with r ≥ k(k+1)/2.
 * For such (r,k)
 *
 *   T(r,k) = k · ⌊ ( r – k(k‑1)/2 ) / k ⌋
 *
 * a(n) is obtained by concatenating the rows of T, skipping zero entries.
 * For each n we locate the row r and column k such that the n‑th element
 * falls in that position, then compute T(r,k) as above.
 *
 * The implementation uses only integer arithmetic and no external libraries
 * beyond the standard C headers. */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Integer square‑root for unsigned 64‑bit values.  Returns ⌊√x⌋. */
static idx_t int_isqrt_u64(idx_t x)
{
    /* Use long double sqrt for an accurate starting point. */
    long double d = sqrt((long double)x);
    idx_t r = (idx_t)d;
    /* Correct possible rounding errors. */
    while (( (__int128)(r+1) * (r+1) ) <= x) ++r;
    while (( (__int128)r * r ) > x) --r;
    return r;
}

/* Compute the greatest k such that r ≥ k(k+1)/2 (i.e. the number of non‑zero
 * entries in row r of the triangle). */
static idx_t row_kmax(idx_t r)
{
    /* Solve k(k+1)/2 ≤ r  →  k = ⌊(√(1+8r) – 1)/2⌋. */
    idx_t s = int_isqrt_u64( (idx_t)1 + ( (__int128)8 * r ) );
    idx_t k = (s - 1) / 2;
    /* Adjust for possible off‑by‑one caused by integer sqrt. */
    while ( ( (__int128)(k+1) * (k+2) ) / 2 <= r ) ++k;
    while ( ( (__int128)k * (k+1) ) / 2 > r ) --k;
    return k;
}

/* Triangle entry as defined above. */
static seq_t triangle_entry(idx_t r, idx_t k)
{
    seq_t r_s  = (seq_t)r;
    seq_t k_s  = (seq_t)k;
    /* Number of admissible starting integers a is floor((r – k(k‑1)/2) / k). */
    seq_t num = r_s - ( k_s * (k_s - 1) ) / 2;
    seq_t count = num / k_s;          /* floor division */
    return k_s * count;               /* total number of parts */
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of entries up to
     * row r-1 is < n ≤ cumulative up to row r. */
    idx_t r = 1;
    idx_t seen = 0;
    while (true) {
        idx_t kmax = row_kmax(r);
        if (seen + kmax >= n) break;
        seen += kmax;
        ++r;
    }
    /* Column index within the row (1‑based). */
    idx_t k = n - seen;
    /* Compute the triangle value for the located (r,k). */
    return triangle_entry(r, k);
}
