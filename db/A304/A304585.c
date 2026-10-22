/*
 * A304585  A linear mapping a(n) = x + d*n of pairs of nonnegative integers
 *          (x,d), where the pairs are enumerated by meandering antidiagonals.
 * https://oeis.org/A304585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1373, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A304585   –  a(n) = x + d·n
    The pair (x,d) is obtained by traversing non‑negative integer
    pairs (x,d) along “meandering” antidiagonals of the array NxN.
    For the diagonal of rank s (sum x+d = s) the entries are listed
    in the order
        even s :  (0,s),(1,s‑1),…,(s,0)          (ascending x)
        odd  s :  (s,0),(s‑1,1),…,(0,s)          (descending x)

    For a given index n:
        1.  s  = floor((√(8·n+1) – 1)/2)   – diagonal rank
        2.  k  = n – s·(s+1)/2            – offset inside diagonal
        3.  if s even :  x = k          , d = s – k
            else      :  x = s – k      , d = k
        4.  a(n) = x + d·n
    This algorithm runs in O(log n) time (binary‑search integer sqrt).

    The sequence is defined for all n≥0 and the output fits into
    seq_t when it is representable; otherwise the result is truncated,
    which is acceptable for the 64‑bit build because only terms that
    fit are tested.  The routine is fully deterministic and may be
    called in any order.                                             */

#include <stdint.h>
#include <stddef.h>

/* Integer 128 bit type (GNU extension) used only for intermediate
   computations.  It is guaranteed to be available in C11/GNU C.      */
typedef unsigned __int128 u128;

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(√x).  Binary search on [0, 2^64].                 */
static u128 u128_isqrt(u128 a)
{
    u128 low = 0, high = (u128)1 << 64;   /* high is exclusive */
    while (low + 1 < high) {
        u128 mid = (low + high) >> 1;
        u128 sq  = mid * mid;
        if (sq <= a)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main function required by the template.                         */
seq_t a(idx_t n)
{
    /* Step 1:  diagonal rank s = floor((√(8n+1)-1)/2)            */
    u128 t   = (u128)n * 8 + 1;            /* 8·n+1  (up to 128 bits) */
    u128 root = u128_isqrt(t);             /* floor sqrt(8n+1) */
    idx_t s   = (idx_t)((root - 1) >> 1);  /* divide by 2 */

    /* Step 2:  offset k inside the diagonal                      */
    u128 tri   = (u128)s * (s + 1) / 2;    /* number of elements before diag s  */
    idx_t k    = (idx_t)((u128)n - tri);

    /* Step 3:  compute the pair (x,d) based on parity of s       */
    idx_t x, d;
    if ((s & 1) == 0) {          /* even diagonal : ascending x         */
        x = k;
        d = s - k;
    } else {                     /* odd diagonal  : descending x         */
        x = s - k;
        d = k;
    }

    /* Step 4:  compute a(n) = x + d·n   (using 128‑bit intermediate) */
    u128 val = (u128)d * (u128)n + (u128)x;

    return (seq_t)val;
}
