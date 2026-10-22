/*
 * A365736  G.f. satisfies A(x) = 1 + x*A(x) / (1 - x^5*A(x)^4).
 * https://oeis.org/A365736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} binomial(n-4*k-1,k) * binomial(n-k,4*k)
 *            / (4*k+1) (The division is exact for each term.)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 31, u64 n <= 57, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute binomial coefficient C(a,b) with a,b >= 0.
   Returns 0 if b > a. Uses multiplicative formula with
   intermediate reductions to keep values small. */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res = res * (seq_t)(a - b + i);   /* multiply first */
        res = res / (seq_t)i;             /* then divide (guaranteed integral) */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 1;            /* term for k = 0 */
    idx_t kmax = n / 5;
    for (idx_t k = 1; k <= kmax; ++k) {
        /* Skip if condition n-4*k-1 < k, then the binomial is zero. */
        if (n < 4 * k + 1) continue;

        /* First binomial: C(n-4*k-1, k) */
        idx_t n1 = n - 4 * k - 1;
        seq_t b1 = binom(n1, k);
        if (b1 == 0) continue;   /* just in case */

        /* Second binomial: C(n-k, 4*k)  (using complement identity) */
        idx_t n2 = n - k;
        idx_t k2 = 4 * k;
        seq_t b2 = binom(n2, k2);
        if (b2 == 0) continue;

        /* Denominator: 4*k + 1 */
        idx_t den = 4 * k + 1;

        /* Compute product / den.  For 64‑bit builds use 128‑bit intermediate
           to avoid overflow, for larger builds use the type of seq_t itself. */
        seq_t term;
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            __uint128_t prod = (__uint128_t)b1 * (__uint128_t)b2;
            term = (seq_t)(prod / (seq_t)den);
        } else {
            term = (b1 * b2) / (seq_t)den;
        }

        total += term;
    }
    return total;
}
