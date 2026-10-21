/*
 * A245677  Numerator of sum of fractions A182972(k) / A182973(k) such that
 *          A182972(k) + A182973(k) = n.
 * https://oeis.org/A245677
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∑ a/(n−a) over all a with 1 ≤ a < n/2 and
 *            gcd(a,n)=1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euclidean algorithm for unsigned 64‑bit integers. */
static inline idx_t gcd_uint(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Euclidean algorithm for seq_t values (works for built‑in integers
   and for arbitrary‑precision types that overload % and ==). */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute OEIS A245677. */
seq_t a(idx_t n)
{
    if (n < 3) {
        return (seq_t)0;  /* undefined for n < 3 */
    }

    seq_t P = (seq_t)0; /* numerator of the sum */
    seq_t Q = (seq_t)1; /* denominator of the sum */

    for (idx_t a_val = 1; a_val * 2 < n; ++a_val) {
        if (gcd_uint(a_val, n) != 1) {
            continue;        /* skip non‑coprime terms */
        }

        /* Current term: a_val / (n - a_val) */
        idx_t denom_ui = n - a_val;
        seq_t denom = (seq_t)denom_ui;

        /* New numerator: P*denom + a_val*Q */
        seq_t newP = P * denom + (seq_t)a_val * Q;
        /* New denominator: Q*denom */
        seq_t newQ = Q * denom;

        /* Reduce the fraction */
        seq_t g = gcd_seq(newP, newQ);
        P = newP / g;
        Q = newQ / g;
    }

    /* Return the numerator of the reduced fraction. */
    return P;
}
