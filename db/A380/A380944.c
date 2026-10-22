/*
 * A380944  a(n) = b(n,A000120(n)) for n >= 0 where b(n,k) is defined in
 *          Comments.
 * https://oeis.org/A380944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A380944:
   a(n) = b(n, popcount(n))
   where b is defined recursively:
     b(2q+1,k) = b(q,k) + b(q,k-1)          (k>0)
     b(2q,  k) = (popcount(q)-k+1) * b(2q+1,k)   (q>0, k>0)
     b(n,0)    = 1   for n>=0
     b(0,k)    = 0   for k>0
*/
#include <stdint.h>

static seq_t b_value(idx_t n, idx_t k)
{
    if (k == 0)                /* base: T(n,0)=1 for all n */
        return (seq_t)1;
    if (n == 0)                /* base: T(0,k)=0 for k>0 */
        return (seq_t)0;

    if (n & 1u) {              /* odd: n = 2q+1 */
        return b_value(n >> 1, k) + b_value(n >> 1, k - 1);
    } else {                   /* even: n = 2q   */
        seq_t odd_val = b_value(n | 1, k);          /* b(2q+1,k) */
        idx_t q = n >> 1;
        idx_t pc = __builtin_popcountll((unsigned long long)q);
        seq_t factor = (seq_t)(pc - k + 1);
        return factor * odd_val;
    }
}

seq_t a(idx_t n)
{
    idx_t pc = __builtin_popcountll((unsigned long long)n);
    return b_value(n, pc);
}
