/*
 * A382630  a(n) is the number of ways that n can be written as b+c*d, where
 *          b, c and d are positive integers and b < c < d.
 * https://oeis.org/A382630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = |{(b,c,d) ∈ ℕ³ : n = b + c*d, b < c < d}|
 *
 * Computes the number of ways to write n as b + c*d with the
 * inequalities b < c < d and b,c,d ∈ ℕ.
 *
 * The routine uses only integer arithmetic and a lightweight
 * integer square‑root helper so it works for any seq_t/idx_t
 * configuration supplied by the build system.
 */

#include <stdint.h> /* for uint64_t if needed by build; harmless otherwise */

/* ---------- helper: integer square root for idx_t (nonnegative) ---------- */
static inline idx_t isqrt_u64(idx_t x)
{
    if (x < 2)
        return x;
    /* initial approximation: use 1/4 of x shifted left, then Newton iteration */
    idx_t g = (x >> 1) + 1;
    idx_t prev;
    do {
        prev = g;
        g = (x / g + g) >> 1;
    } while (g < prev);
    /* adjust if overshoot */
    while (g * g > x)
        g--;
    return g;
}

/* ---------- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* If n < 7 there is no solution: b=1,c=2,d=3 gives minimum n=7 */
    if (n < 7)
        return 0;

    seq_t count = 0;

    /* maximum possible b from inequality b + (b+1)(b+2) <= n */
    idx_t sqrt_term = isqrt_u64(n + 2);
    idx_t maxb = (sqrt_term > 2) ? sqrt_term - 2 : 0;
    if (maxb > n - 1)
        maxb = n - 1;  /* b cannot exceed n-1 */

    for (idx_t b = 1; b <= maxb; ++b) {
        idx_t rem = n - b;                  /* remaining part to be factored as c*d */
        if (rem <= 1)                       /* need at least c >= 2 and d >=3 */
            continue;

        idx_t limC = isqrt_u64(rem - 1);    /* c must satisfy c^2 < rem to have d > c */
        if (limC <= b)
            continue;

        for (idx_t c = b + 1; c <= limC; ++c) {
            if (rem % c != 0)
                continue;
            idx_t d = rem / c;
            if (d > c)                      /* ensures b < c < d */
                ++count;
        }
    }
    return count;
}
