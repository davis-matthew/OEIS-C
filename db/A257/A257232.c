/*
 * A257232  Triangle T(n, k) with the natural numbers in columns with nonprime
 *          k and the nonnegative numbers in columns with prime k, for 1 <= k
 *          <= n.
 * https://oeis.org/A257232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = i – (j-1) – is_prime(j) where * i = minimal integer such
 *            that i(i+1)/2 >= n * j = n – i(i-1)/2 * (i,j) gives row and
 *            column of the triangular array * T(i,j) = i – (j-1) –
 *            is_prime(j) (T is the OEIS triangle A257232) * This enumerates
 *            the triangle row‑by‑row and returns the * corresponding...
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Find the row i via binary search on i(i+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = n;          /* high := n is never too large for __int128 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1u) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1u;
    }

    idx_t i = low;
    unsigned __int128 prev_total = (unsigned __int128)i * (i - 1u) / 2;
    idx_t j = n - (idx_t)prev_total;          /* 1 <= j <= i */

    bool prime = is_prime(j);                 /* 1 if j is prime, else 0 */

    return (seq_t)i - (seq_t)(j - 1u) - (seq_t)prime;
}
