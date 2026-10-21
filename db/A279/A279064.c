/*
 * A279064  Numbers n such that the sum of numbers less than n that do not
 *          divide n is even.
 * https://oeis.org/A279064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that the sum of positive integers
 *            < k that do not divide k is even. Equivalently, k ∈ A279064 ⇔
 *            k(k+1)/2 and σ(k) have the same parity
 * Range:     u8 n <= 129, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the integer square root of an unsigned 64‑bit integer.
   The result r satisfies r*r ≤ n < (r+1)*(r+1). */
static idx_t
isqrt_u64(idx_t n)
{
    idx_t low = 0;
    /* 2^32 = 4294967296 > √(2^64‑1), so the search range is safe. */
    idx_t high = ((idx_t)1) << 32;    /* exclusive upper bound */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Does k belong to the sequence A279064?  */
static bool
is_member(idx_t k)
{
    /* Parity of k(k+1)/2 : odd when k ≡ 1 or 2 (mod 4). */
    bool parity_t = ((k & 3) == 1 || (k & 3) == 2);

    /* Strip powers of two to obtain the odd part. */
    idx_t odd = k;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* σ(k) is odd ⇔ the odd part is a perfect square. */
    idx_t r = isqrt_u64(odd);
    bool odd_is_square = (r * r == odd);

    /* Membership holds when the two parities are equal. */
    return parity_t == odd_is_square;
}

/* Return the n‑th term of A279064 (a(1) = 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cand   = 0;

    while (count < n) {
        ++cand;
        if (is_member(cand))
            ++count;
    }

    return (seq_t)cand;
}
