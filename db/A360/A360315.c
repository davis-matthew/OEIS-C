/*
 * A360315  a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binomial(n-1-3*k,n-4*k) *
 *          binomial(2*k,k).
 * https://oeis.org/A360315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} (-1)^k * binomial(n-1-3k, n-4k) *
 *            binomial(2k, k)
 * Range:     i8 n <= 14, i16 n <= 28, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n, k) for 0 <= k <= n using multiplicative
   algorithm. Returns 0 when k > n. */
static seq_t binomial_custom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;             /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply first, then divide. Use 128‑bit intermediate to avoid
           overflow when seq_t is 64‑bit. */
        res = (seq_t)(((__int128)res * (seq_t)(n - k + i)) / (seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle the sole special case n = 0 explicitly. */
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t maxk = n / 4;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* First binomial: C(n-1-3k, n-4k). The arguments are non‑negative
           for all k in the loop because n >= 4k. */
        idx_t top   = n - 1 - 3 * k;
        idx_t bottom = n - 4 * k;
        seq_t b1 = binomial_custom(top, bottom);

        /* Second binomial: C(2k, k). */
        seq_t b2 = binomial_custom(2 * k, k);

        /* Product term. Use 128‑bit intermediate for multiplication. */
        seq_t term = (seq_t)(((__int128)b1 * b2));

        /* Add or subtract according to (-1)^k. */
        if (k & 1) sum -= term;
        else       sum += term;
    }
    return sum;
}
