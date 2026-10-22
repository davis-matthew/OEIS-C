/*
 * A380920  Triangle read by rows: Define function b(n,k,i) where b(n,k,1) =
 *          n/k and b(n,k,i+1) = (i*b(n,k,i))/floor(i*b(n,k,i)). T(n,k) is the
 *          smallest number i such that b(n,k,i) = 1.
 * https://oeis.org/A380920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 850, u32 n <= 2962, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A380920: Triangle read by rows.
 *
 * For 1 ≤ k ≤ n, define
 *   b(n,k,1) = n/k
 *   b(n,k,i+1) = (i*b(n,k,i)) / floor(i*b(n,k,i))
 *
 * Let T(n,k) be the smallest i for which b(n,k,i) = 1.
 * The sequence a(n) is the triangle of T(n,k) read by rows,
 * i.e. a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 *
 * This implementation finds the row and column corresponding
 * to the n‑th term, then computes T(n,k) by iterating the recurrence.
 *
 * Greedy algorithm: For fixed n,k compute the fraction as a reduced
 * numerator/denominator (num/den).  At step i check whether num==den;
 * otherwise compute f = floor(i*num/den), then
 *   new_num = i*num          (unreduced)
 *   new_den = den * f
 * Reduce new_num/new_den by gcd, set i++ and repeat.
 * When num==den we terminate and return i.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Helper: compute T(n,k) as defined above.
   n >= k >= 1 */
static seq_t triangle_T(idx_t n, idx_t k)
{
    seq_t num = (seq_t)n;
    seq_t den = (seq_t)k;
    idx_t i = 1;

    while (num != den) {
        /* f = floor(i * num / den)  */
        seq_t f = (seq_t)i * num / den;
        seq_t new_num = (seq_t)i * num;
        seq_t new_den = den * f;

        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
        ++i;
    }
    return (seq_t)i;
}

/* Main function: a(n) for the OEIS sequence A380920.
 * n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* index 0 is outside the offset, behaviour undefined */
        return (seq_t)0;
    }

    /* Find the row r such that r(r+1)/2 ≥ n and (r-1)r/2 < n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;

    /* Column k = n - (row-1)row/2 */
    unsigned __int128 prev = (unsigned __int128)(row - 1) * row / 2;
    idx_t col = n - (idx_t)prev;

    /* Compute the triangle entry T(row,col) */
    return triangle_T(row, col);
}
