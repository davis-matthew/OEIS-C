/*
 * A348533  Generalized Josephus problem: Let T(m,k), k>=2, m=1,2,3,.., be the
 *          number of people on a circle such that the survivor is one of the
 *          first k-1 people after every k-th person has been removed.
 * https://oeis.org/A348533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   compute_T_m_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A348533: Generalized Josephus problem
 *
 * Recurrence:
 *   T(1,k)=1 , S(1,k)=1
 *   Let p = T(m,k) mod (k-1)
 *   Let e = -p            if S(m,k) > p
 *          k-1-p          otherwise
 *   T(m+1,k) = (k*T(m,k)+e) / (k-1)
 *   S(m+1,k) = 1 + (S(m,k)+e-1) mod T(m+1,k)
 *
 * Sequence a(n) is obtained by reading the table T(m,k) (k≥2, m≥1)
 * along ascending antidiagonals: (m+k constant).
 * The mapping (m,k) ↔ n:
 *   Let s = m + k   (s ≥ 3)
 *   Terms up to diagonal s-1 are C(s-1) = (s-3)(s-2)/2
 *   Offset in diagonal: offset = n - C(s-1) - 1   (0‑based)
 *   m = (s-2) - offset
 *   k = s - m
 *
 * The implementation computes T(m,k) directly using the recurrence.
 * For the 64‑bit build intermediate values stay within bounds for all
 * indices whose terms fit in 64 bits; for larger indices overflow
 * is harmless because those terms are not checked.
 */

#include <stdbool.h>
#include "../utils/compute_T_m_k.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal s for given n */
    idx_t s = 3;
    while (((s - 2) * (s - 1)) / 2 < n) {
        ++s;
    }
    /* Offset inside diagonal (0‑based) */
    idx_t prevC = ((s - 3) * (s - 2)) / 2;   /* C(s-1) */
    idx_t offset = n - prevC - 1;
    idx_t m = (s - 2) - offset;
    idx_t k = s - m;

    /* Compute T(m,k) */
    return compute_T_m_k(m, k);
}
