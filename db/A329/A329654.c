/*
 * A329654  a(n) = numerator(b(n)), where b(0) = b(1) = 1 and b(n) =
 *          n*b(n-1)/b(n-2) for n >= 1.
 * https://oeis.org/A329654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(b(n)) where b(0) = b(1) = 1 and b(n) =
 *            n*b(n-1)/b(n-2) for n >= 2. The function keeps numerators and
 *            denominators reduced by gcd at each step, avoiding overflow for
 *            small seq_t types
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 40, u64 n <= 82, u128 n <= 151,
 *            big any n
 * Helpers:   seq_gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"
#include "../utils/seq_realloc.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cached arrays for numerators (p) and denominators (q) */
    static seq_t *p = NULL;
    static seq_t *q = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static idx_t max_calculated = 0;/* highest index computed so far */
    static bool initialized = false;

    /* One‑time allocation and base terms */
    if (!initialized) {
        size_t init_cap = 32;          /* arbitrary small start size */
        p = seq_realloc(p, 0, init_cap);
        q = seq_realloc(q, 0, init_cap);
        cap = init_cap;

        /* b(0) = 1/1, b(1) = 1/1 */
        p[0] = 1;  q[0] = 1;
        p[1] = 1;  q[1] = 1;
        max_calculated = 1;
        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= max_calculated)
        return p[n];

    /* Grow storage if necessary */
    size_t new_cap = cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                /* double capacity */
    if (new_cap > cap) {
        p = seq_realloc(p, cap, new_cap);
        q = seq_realloc(q, cap, new_cap);
        cap = new_cap;
    }

    /* Compute new terms using the recurrence on reduced fractions */
    for (idx_t i = max_calculated + 1; i <= n; ++i) {
        seq_t a_val = (seq_t)i;          /* factor n  */
        seq_t b = p[i - 1];              /* numerator of b(i-1) */
        seq_t c = q[i - 2];              /* denominator of b(i-2) */
        seq_t d = q[i - 1];              /* denominator of b(i-1) */
        seq_t e = p[i - 2];              /* numerator of b(i-2) */

        /* Cancel common factors to keep intermediate products bounded */
        seq_t g;

        g = seq_gcd(b, e);   b /= g;   e /= g;
        g = seq_gcd(d, c);   d /= g;   c /= g;
        g = seq_gcd(a_val, e); a_val /= g; e /= g;
        g = seq_gcd(a_val, d); a_val /= g; d /= g;

        /* Compute numerator and denominator */
        seq_t num = a_val;
        num *= b;
        num *= c;

        seq_t den = d;
        den *= e;

        /* Final reduction to keep fraction in lowest terms */
        g = seq_gcd(num, den);
        p[i] = num / g;
        q[i] = den / g;
    }

    max_calculated = n;
    return p[n];
}
