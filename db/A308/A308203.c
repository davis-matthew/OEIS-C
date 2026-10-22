/*
 * A308203  Array read by ascending antidiagonals: T(n,k) = number of
 *          non-isomorphic kC_n-snake graphs for n >= 3 and k >= 2.
 * https://oeis.org/A308203
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 115, u32 n <= 268, u64 n <= 654,
 *            u128 n <= 1277, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n): read the array T(n,k) = (⌊n/2⌋^(k‑2) + ⌊n/2⌋^⌊(k‑1)/2⌋)/2
 *       along ascending antidiagonals with offset 3.  
 * The function translates the index n (≥ 3) to the corresponding pair (row,column)
 * in the array and evaluates T(row,column) directly using integer arithmetic.
 */
 
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp); */
 
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3.  For n < 3 the value is
       unspecified; return 0 for safety. */
    if (n < 3)
        return 0;

    /* Position within the sequence (a(3) is first). */
    idx_t idx = n - 2;

    /* Find m such that m*(m+1)/2 ≥ idx; let S = m + 4 be the sum
       (n + k) of the current diagonal. */
    idx_t m = 0;
    while (m * (m + 1) / 2 < idx)
        ++m;
    idx_t S = m + 4;                     /* n + k for this diagonal */
    
    /* Number of terms before this diagonal: (m-1)*m/2. */
    idx_t prev = m * (m - 1) / 2;
    
    /* Position inside the diagonal (1‑based). */
    idx_t p = idx - prev;
    idx_t k = p + 1;                     /* column index (k ≥ 2) */
    seq_t row = (seq_t)(S - k);          /* row index (n ≥ 3) */
    
    /* floor(n/2) */
    seq_t half = row >> 1;
    
    /* Exponents for the two powers in the formula. */
    idx_t exp1 = k - 2;
    idx_t exp2 = (k - 1) >> 1;           /* floor((k-1)/2) */
    
    seq_t term1 = big_pow(half, (seq_t)exp1);
    seq_t term2 = big_pow(half, (seq_t)exp2);
    
    seq_t sum = term1 + term2;
    return sum >> 1;                     /* divide by 2 */
}
