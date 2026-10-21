/*
 * A288535  Array RATS(b,n) by upward antidiagonals: Reverse Add Then Sort
 *          digits of n>0 in base b>1.
 * https://oeis.org/A288535
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* RATS sequence (A288535)
 * Definition: For each pair (b, n) with b ≥ 2, n ≥ 1,
 * compute the number obtained by reversing the base‑b digits of n,
 * adding the two numbers, sorting the sum's base‑b digits, and
 * interpreting the sorted digits as a base‑b number.
 * The infinite array is enumerated by upward antidiagonals:
 * the pairs are ordered by increasing sum S = b + n;
 * for each S ≥ 3 the pairs (b, n) are listed with b decreasing
 * from S‑1 to 2, giving n = S – b.
 * a(2) is the first term of this enumeration.
 *
 * The function below computes a(n) for any n ≥ 2 in O(√n) time
 * without memoization, suitable for quick evaluations up to several
 * thousand indices.  The implementation uses only integer arithmetic
 * allowed by the build (C11/C++17) and respects the seq_t type.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* behavior below the offset is irrelevant */

    /* 1. Find the antidiagonal S and the position r within it. */
    idx_t k = n - 2;                /* zero‑based index of the pair */

    idx_t m = 1;
    /* m satisfies m(m+1)/2 > k ;  m = (S-2) */
    while ((m * (m + 1)) / 2 <= k) ++m;

    idx_t pre = (m - 1) * m / 2;      /* total pairs before this antidiagonal */
    idx_t r   = k - pre;              /* 0‑based offset inside the diagonal */

    idx_t base = (m + 1) - r;        /* b = S‑1 - r,  guaranteed ≥ 2 */
    idx_t col  = r + 1;              /* n = r+1, guaranteed ≥ 1 */

    /* 2. Compute reverse‑add‑then‑sort for (base, col). */
    seq_t base_seq = (seq_t)base;
    seq_t n_val    = (seq_t)col;

    /* Reverse the base‑digits of n_val. */
    seq_t rev = 0;
    seq_t tmp = n_val;
    while (tmp > 0) {
        seq_t digit = tmp % base_seq;
        rev = rev * base_seq + digit;
        tmp /= base_seq;
    }

    /* Sum and convert to digits. */
    seq_t sum = n_val + rev;

    seq_t digits[200];
    idx_t len = 0;
    if (sum == 0) {
        digits[len++] = 0;          /* keep a zero if the sum is zero */
    } else {
        seq_t s = sum;
        while (s > 0 && len < 200) {
            digits[len++] = s % base_seq;
            s /= base_seq;
        }
    }

    /* Sort digits in ascending order (insertion sort, small size). */
    for (idx_t i = 1; i < len; ++i) {
        seq_t key = digits[i];
        idx_t j = i;
        while (j > 0 && digits[j - 1] > key) {
            digits[j] = digits[j - 1];
            --j;
        }
        digits[j] = key;
    }

    /* Reassemble number from sorted digits (most significant first). */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        result = result * base_seq + digits[i];
    }

    return result;
}
