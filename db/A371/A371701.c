/*
 * A371701  a(n) is the least positive k such that Product_{i=1..k} 1 / (1 -
 *          1/(2*i+1)) >= n.
 * https://oeis.org/A371701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the smallest positive k such that
 * ∏_{i=1..k} (2i+1)/(2i)  ≥  n.
 *
 * Implementation uses a binary search on k together with high‑precision
 * logarithmic evaluation of the product via lgammal, so no floating
 * arithmetic is performed on values of type seq_t.
 */
#include <stdint.h>
#include <math.h>

/* Compute log of the product P_k = ∏_{i=1..k} (2i+1)/(2i)
 * using the closed form P_k = (2k+1)! / (4^k * (k!)^2).
 */
static inline long double log_product(idx_t k)
{
    if (k == 0)
        return 0.0L;                     /* product = 1 */
    /* log((2k+1)!) = lgamma(2k+2) */
    long double log_num = lgammal(2 * k + 2);
    /* log(4^k) = k * log(4) = 2k * log(2) */
    long double log_den = 2 * (long double)k * logl(2.0L);
    /* 2 * log(k!) = 2 * lgamma(k+1) */
    log_den += 2 * lgammal(k + 1);
    return log_num - log_den;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the offset – a(0) = a(1) = 1 */
    if (n <= 1U)
        return (seq_t)1;

    long double log_n = logl((long double)n);

    /* Estimate an upper bound for k: a(n) ~ π * n² / 4 */
    long double pi = acosl(-1.0L);
    idx_t high = (idx_t)((pi * (long double)n * (long double)n) / 4.0L) + 10;
    if (high == 0)
        high = 10;        /* safety for tiny n */

    idx_t low = 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        long double logP = log_product(mid);
        if (logP >= log_n)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
