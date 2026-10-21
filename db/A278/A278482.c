/*
 * A278482  Square array A(n,k): A(0, n) = n; A(k, n) = A(k-1,
 *          floor(n*(k+1)/k)), for k >= 1, read by descending antidiagonals as
 *          A(0,0), A(0,1), A(1,0), A(0,2), A(1,1), A(2,0), ...
 * https://oeis.org/A278482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 640, u16 n <= 9785, u32 n <= 9785, u64 n <= 9785,
 *            u128 n <= 9785, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278482:
   Square array A(k,n) with A(0,n)=n,
   A(k,n)=A(k-1, floor(n*(k+1)/k)) for k>0.
   The sequence is read by descending antidiagonals. */
/* 0,1,0,2,2,0,3,4,2,0,4,6,6,2,0,5,8,8,6,2,0,6,10,12, ... */

#include <stdint.h>
#include <math.h>

/* triangular number T(s) = s*(s+1)/2, computed with 128‑bit arithmetic
   for safety when s is large. */
static inline idx_t triangular(idx_t s)
{
    return (idx_t)(((__int128)s * (s + 1)) >> 1);
}

seq_t a(idx_t n)
{
    /* Find diagonal s where T(s) <= n < T(s+1). */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct for any rounding error. */
    while (triangular(s) > n) s--;
    while (triangular(s + 1) <= n) s++;

    idx_t cum = triangular(s);          /* index of first element of diag s */
    idx_t offset = n - cum;             /* position within diagonal (k) */
    idx_t k = offset;                   /* number of recurrence steps */
    idx_t i = s - offset;               /* the starting n in A(k,i) */

    /* Compute A(k,i) by iterating the recurrence in reverse order. */
    seq_t value = (seq_t)i;
    idx_t step = k;
    while (step > 0) {
        value = (value * (seq_t)(step + 1)) / (seq_t)step;
        step--;
    }
    return value;
}
