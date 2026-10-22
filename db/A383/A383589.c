/*
 * A383589  a(n) = A378762(A381662(n)).
 * https://oeis.org/A383589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the value of the n‑th element in a self‑inverse permutation
   defined by the triangular array T(n,k) given by:
       T(n,k) = S(n-1) + P(n,k)
   where S(r) = 2*r^2 – r is the starting integer of row r (row 1 starts at 1),
   m = 2*n‑1, and
       P(n,k) =  k                     if k <  m and k odd
                = m+1‑k                 if k <  m and k even
                = 3*m‑1‑k                if k ≥  m and k odd
                =  k                     if k ≥  m and k even
   The function finds the row r containing the n‑th element by solving
   S(r) ≥ n, then computes the offset k = n – S(r‑1) and returns T(n,k). */

#pragma once
/* Assuming seq_t and idx_t are defined elsewhere. */

seq_t a(idx_t n)
{
    /* Find smallest r such that S(r) = 2*r^2 – r >= n */
    idx_t low = 1, high = 1;
    /* Expand high until >= n */
    while (1) {
        seq_t S_high = (seq_t)2 * (seq_t)high * (seq_t)high - (seq_t)high;
        if (S_high >= n) break;
        high <<= 1;                           /* r will never exceed n, so this is safe */
    }
    /* Binary search for r */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t S_mid = (seq_t)2 * (seq_t)mid * (seq_t)mid - (seq_t)mid;
        if (S_mid >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    /* Compute S(r-1) and k */
    seq_t S_prev = (seq_t)2 * (seq_t)(r - 1) * (seq_t)(r - 1) - (seq_t)(r - 1);
    idx_t k = n - (idx_t)S_prev;               /* position inside the row (1 ≤ k ≤ 4r-3) */

    /* Compute P based on the rules */
    idx_t m = 2 * r - 1;
    idx_t P;
    if (k < m) {
        if (k & 1)          /* odd */
            P = k;
        else
            P = m + 1 - k;
    } else {
        if (k & 1)          /* odd */
            P = 3 * m - 1 - k;
        else
            P = k;
    }

    /* Final value: T(r,k) = S(r-1) + P */
    return S_prev + (seq_t)P;
}
