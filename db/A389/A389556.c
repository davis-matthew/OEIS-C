/*
 * A389556  The number of primitive Pythagorean triples (PPTs) that lie within
 *          the first n rows of an irregular triangular array of PPTs defined
 *          in comments and whose even leg of the integer right triangle
 *          formed from the PPT is greater than its odd leg.
 * https://oeis.org/A389556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{n} count(m) where for row m (k = 2m+1) count(m)
 *            = |{ y : ceil( k/(2+√2) ) ≤ y ≤ (k-1)/2 , gcd(k,y)=1 }|
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation performs a direct summation for each call.  
   No dynamic allocation is used, so it works for both plain integer
   and arbitrary‑precision seq_t.  */

#include <limits.h>
#include <math.h>
#include "../utils/gcd.h"

/* --------------------------------------------------------------------- */
/*  Count admissible y for a given odd k.                                 */
/* --------------------------------------------------------------------- */
static inline seq_t
count_k(idx_t k)
{
    /*  Compute lower bound y_min = ceil(k/(2+√2))  */
    static const long double inv = 1.0L / (2.0L + sqrtl(2.0L));
    idx_t y_min = (idx_t) ceill((long double)k * inv);   /* ceil */

    /*  Upper bound y_max = floor((k-1)/2)  */
    idx_t y_max = (k - 1) / 2;

    seq_t cnt = 0;
    for (idx_t y = y_min; y <= y_max; ++y) {
        if (gcd((seq_t)k, (seq_t)y) == 1)
            ++cnt;
    }
    return cnt;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not defined below offset, but safe */

    seq_t total = 0;
    for (idx_t m = 1; m <= n; ++m) {
        idx_t k = 2 * m + 1;          /* odd number for this row      */
        total += count_k(k);          /* add admissible triplets       */
    }
    return total;
}
