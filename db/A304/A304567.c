/*
 * A304567  Number of maximum irredundant sets in the n-antiprism graph.
 * https://oeis.org/A304567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 576, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   If n mod 3 = 0          ->  a(n) = 15
 *   If n mod 3 = 1 (n = 3k+1) -> a(n) = (3k+1)*(k+1)*(5k²+10k+6)/6
 *   If n mod 3 = 2 (n = 3k+2) -> a(n) = 2*(k+1)*(3k+2)
 *   (where k = ⌊n/3⌋)
 * This implements OEIS sequence A304567 – number of maximum irred­undant
 * sets in the n‑antiprism graph.
 */
#include <stdint.h>   /* for C99 integer types if needed */

seq_t a(idx_t n)
{
    /* Offset: a(1) is first term.  For n <= 0 behaviour is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    idx_t mod = n % 3;

    if (mod == 0) {
        /* a(3k) = 15 */
        return (seq_t)15;
    } else if (mod == 1) {
        /* a(3k+1) = (3k+1)*(k+1)*(5k^2+10k+6)/6 */
        idx_t k = (n - 1) / 3;
        seq_t kk = (seq_t)k;
        seq_t nseq = (seq_t)n;
        seq_t term1 = nseq;                /* 3k+1 */
        seq_t term2 = kk + 1;              /* k+1 */
        seq_t term3 = 5 * kk * kk + 10 * kk + 6;  /* 5k^2 + 10k + 6 */
        seq_t prod = term1 * term2;
        prod = prod * term3;
        return prod / 6;
    } else {
        /* mod == 2  ->  a(3k+2) = 2*(k+1)*(3k+2) */
        idx_t k = (n - 2) / 3;
        seq_t kk = (seq_t)k;
        seq_t nseq = (seq_t)n;   /* 3k+2 */
        return 2 * (kk + 1) * nseq;
    }
}
