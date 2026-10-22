/*
 * A304108  Numbers n such that the (0,1)-polynomial encoded in binary
 *          expansion of n has at least one duplicated irreducible divisor
 *          when the factorization is done in polynomial ring GF(2)[X].
 * https://oeis.org/A304108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 8017, u32 n <= 8017, u64 n <= 8017,
 *            u128 n <= 8017, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304108: nth smallest integer n such that the (0,1)-polynomial
   encoded in the binary expansion of n has a repeated irreducible
   divisor in GF(2)[X].  The value is found by a simple brute‑force
   search.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the degree (highest set bit index) of a polynomial represented
   by a seq_t.  For zero the degree is defined as 0.  */
static size_t poly_degree(seq_t x)
{
    if (x == (seq_t)0) return 0;
    size_t d = 0;
    while (x) {
        x >>= 1;
        ++d;
    }
    return d - 1;
}

/* Greatest common divisor of two polynomials over GF(2).  */
static seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        if (a == (seq_t)0) return b;
        size_t da = poly_degree(a);
        size_t db = poly_degree(b);
        if (da < db) {
            seq_t t = a;
            a = b;
            b = t;
        } else {
            a ^= b << (unsigned int)(da - db);
        }
    }
    return a;
}

/* Checks whether n has a repeated irreducible factor in its binary
   polynomial over GF(2).  */
static bool repeated_factor(seq_t n)
{
    if (n <= (seq_t)1) return false;   /* 0 and 1 never qualify */

    /* Derivative of f(x): sum_{i odd} x^{i-1}  */
    seq_t derivative = (seq_t)0;
    seq_t temp = n;
    unsigned pos = 0;
    while (temp) {
        if (temp & (seq_t)1) {            /* bit is 1 */
            if (pos & 1) {                /* index odd -> contributes x^{pos-1} */
                if (pos > 0)
                    derivative |= (seq_t)1 << (pos - 1);
            }
        }
        temp >>= 1;
        ++pos;
    }

    seq_t g = poly_gcd(n, derivative);
    return g != (seq_t)1;
}

/* Return the nth term (1‑based) of sequence A304108.  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1, unused otherwise */

    idx_t count = 0;
    seq_t candidate = (seq_t)1;

    while (count < n) {
        if (repeated_factor(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
    /* Unreachable – the loop above always returns. */
    return (seq_t)0;
}
