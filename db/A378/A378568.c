/*
 * A378568  Lowest weight of rational fraction with denominator n.
 * https://oeis.org/A378568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = min_{d|n, d>1} A178031(d) for n>1; a(1) = 1.
   A178031(d) is the minimal continued‑fraction weight of a reduced fraction
   with denominator d (only odd d are relevant).  The implementation below
   precomputes A178031 up to the maximum n encountered and then evaluates
   a(n) by searching the odd divisors of n.  The routine is valid for any
   compliant seq_t and idx_t types. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

/* --------- small utilities ------------------------------------------- */

/* --------- storage of minimal weights for odd denominators ----------- */
static seq_t *A178 = NULL;          /* array indexed by denominator */
static idx_t   A178_last = 0;       /* largest index already computed */

/* compute minimal weights A178031(d) for all odd d in (A178_last, limit] */
static void compute_A178_up_to(idx_t limit)
{
    if (A178_last >= limit) return;

    size_t new_size = (size_t)(limit + 1);
    seq_t *new_ptr = (seq_t *)realloc(A178, new_size * sizeof(seq_t));
    if (!new_ptr) exit(1);           /* allocation failure */
    A178 = new_ptr;

    for (idx_t d = A178_last + 1; d <= limit; ++d) {
        if (d <= 1 || ((d & 1) == 0)) {     /* ignore n <= 1 and even d */
            A178[d] = 0;
            continue;
        }

        /* if d is divisible by 3, a fraction reduces to 1/3 → weight 3 */
        if (d % 3 == 0) {
            A178[d] = (seq_t)3;
            continue;
        }

        /* brute force search over coprime numerators 1..d-1 */
        seq_t best = (seq_t)d + 1;          /* sentinel larger than any weight */
        for (idx_t m = 1; m < d; ++m) {
            if (gcd_u64(m, d) != 1) continue;

            seq_t a_val = (seq_t)m;
            seq_t b_val = (seq_t)d;
            seq_t w = 0;

            /* continued‑fraction sum of partial quotients */
            while (a_val != 0) {
                seq_t q = b_val / a_val;
                w += q;
                seq_t r = b_val % a_val;
                b_val = a_val;
                a_val = r;
            }

            if (w < best) {
                best = w;
                if (best == (seq_t)3) break;   /* cannot get lower */
            }
        }
        A178[d] = best;
    }

    A178_last = limit;
}

/* --------- main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    if ((n & 1) == 0) return (seq_t)2;   /* even denominators are always 2 */

    compute_A178_up_to(n);                /* ensure all needed A178 values are ready */

    seq_t ans = (seq_t)n + 1;             /* large sentinel */

    /* examine all odd divisors of n */
    for (idx_t i = 1; (idx_t)(i * i) <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 > 1 && (d1 & 1)) {
                seq_t v = A178[d1];
                if (v < ans) ans = v;
            }
            if (d2 != d1 && d2 > 1 && (d2 & 1)) {
                seq_t v = A178[d2];
                if (v < ans) ans = v;
            }
        }
    }
    return ans;
}
