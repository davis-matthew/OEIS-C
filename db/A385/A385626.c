/*
 * A385626  Table read by rows: T(n, k) = binomial(n, k) * fibonomial(n, k).
 * https://oeis.org/A385626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑major read of T(n,k) where T(n,k) = binomial(n,k) *
 *            fibonomial(n,k)
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/fib.h"

/* Compute the Fibonomial coefficient {n choose k}_F = n!_F / (k!_F (n-k)!_F) */
/* The expression is product_{i=1..k} F_{n-k+i} / F_i, which remains integer at each step. */
static seq_t fibonomial(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;            /* symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = fib(n - k + i);      /* Fibonacci number, 1–based */
        seq_t den = fib(i);
        result = (result * num) / den;   /* division is exact */
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine the row r such that the cumulative number of entries up to
       row r exceeds n.  Row r has r+1 entries.  */
    idx_t remaining = n;
    idx_t r = 0;
    while (remaining >= (r + 1)) {
        remaining -= (r + 1);
        ++r;
    }
    idx_t c = remaining;               /* column index in row r */

    seq_t bin = binomial(r, c);
    seq_t f  = fibonomial(r, c);
    return bin * f;                    /* exact multiplication in seq_t */
}
