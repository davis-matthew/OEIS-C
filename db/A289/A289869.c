/*
 * A289869  Square array T(n,k) (n>=0, k>=0) read by antidiagonals downwards:
 *          T(n,k) = A005836(n) + 2*A005836(k).
 * https://oeis.org/A289869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = A005836(i) + 2*A005836(j) where (i,j) are the
 *            antidiagonal coordinates of n (0‑based index: m = n-1, d =
 *            diagonal number, offset = m - (d*(d+1)/2), i = offset, j = d -
 *            offset)
 * Range:     u8 n <= 528, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 85 terms through n = 5016 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A005836(x): interpret binary bits of x as digits in base 3. */
static inline seq_t a005836(idx_t x)
{
    seq_t result = 0;
    seq_t pow = 1;                /* 3^position */
    while (x)
    {
        if (x & 1U)
            result += pow;
        pow *= 3;
        x >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n≥1 by the sequence specification. */
    idx_t m = n - 1;              /* 0‑based linear index */
    idx_t d = 0;                  /* diagonal number */
    idx_t terms_before = 0;       /* terms before current diagonal */

    while (terms_before + d + 1 <= m)
    {
        terms_before += d + 1;
        d++;
    }

    idx_t offset = m - terms_before;   /* position within diagonal 0..d */
    idx_t i = offset;                  /* row index */
    idx_t j = d - offset;              /* column index */

    seq_t val = a005836(i);            /* A005836(i) */
    seq_t tmp = a005836(j);            /* A005836(j) */
    val += tmp + tmp;                  /* +2*A005836(j) */

    return val;
}
