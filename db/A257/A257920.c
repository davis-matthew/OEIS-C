/*
 * A257920  Expansion of phi(x) * psi(x^3) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A257920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 990 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A257920: expansion of phi(x) * psi(x^3) in powers of x
   phi(q) = Σ_{m∈ℤ} q^{m^2},    psi(q) = Σ_{k≥0} q^{k(k+1)/2}
   => a(n) = number of integer solutions (m,k) to
        m^2 + 3*k*(k+1)/2 = n
   Counting m==0 once, m>0 twice.  Computed in O(√n) per call.
*/

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t k = 0;            /* current k in psi-series  */
    seq_t s = 0;            /* value of 3*k(k+1)/2 for current k */
    
    /* iterate while s ≤ n */
    while (s <= n)
    {
        seq_t a_term = n - s;          /* remaining value to be a perfect square? */
        
        if (a_term == 0)
            result += 1;               /* m = 0 gives one solution */
        else
        {
            seq_t r = floor_sqrt(a_term);   /* integer square root */
            if (r * r == a_term)            /* perfect square: two m values */
                result += 2;
        }
        
        /* prepare next k: s_{k+1} = s_k + 3*(k+1) */
        k++;
        s += (seq_t)(3 * k);
    }
    
    return result;
}
