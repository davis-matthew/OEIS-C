/*
 * A243756  Triangle read by rows: T(n,k) = A242954(n)/(A242954(k) *
 *          A242954(n-k)).
 * https://oeis.org/A243756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4^(#carries in the base‑4 addition of k and r−k), where r
 *            and k satisfy n = r*(r+1)/2 + k (0 ≤ k ≤ r). This is the
 *            flattened triangle reading T(r,k) =
 *            A242954(r)/(A242954(k)*A242954(r−k))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in row r. */
    idx_t r = 0;
    while ((((__int128)(r + 1) * (r + 2)) / 2) <= n)
        ++r;

    /* Compute the column k within that row. */
    idx_t start = ( (__int128) r * (r + 1) ) / 2;
    idx_t k = n - start;

    /* Count carries in base‑4 addition of k and r−k. */
    idx_t a_val = k;
    idx_t b_val = r - k;
    idx_t carry = 0;
    idx_t carries = 0;

    while (a_val > 0 || b_val > 0)
    {
        idx_t digit_a = a_val % 4;
        idx_t digit_b = b_val % 4;
        idx_t sum = digit_a + digit_b + carry;

        if (sum >= 4)
        {
            ++carries;
            carry = 1;
        }
        else
        {
            carry = 0;
        }

        a_val /= 4;
        b_val  /= 4;
    }

    return pow4((idx_t)carries);
}
