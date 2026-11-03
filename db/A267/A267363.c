/*
 * A267363  a(0) = 0; a(odd n) = 2n+1; a(even n) = a(n/2 - a(n-1)).
 * https://oeis.org/A267363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; a(odd n) = 2*n + 1; a(even n) = a(n/2 - a(n-1))
 * Range:     i8 n <= 25, i16 n <= 409, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 73 terms through n = 9585 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* recursive helper that can handle negative indices */
static seq_t _A(int64_t n)
{
    if (n == 0)
        return 0;                     /* base case */

    if ((n & 1) != 0)                  /* odd index */
        return (seq_t)(2 * n + 1);

    /* even index */
    seq_t prev = _A(n - 1);            /* a(n-1) */
    int64_t m  = n / 2 - (int64_t)prev;/* n/2 - a(n-1) */
    return _A(m);                      /* a(m) */
}

/* public entry point with unsigned idx_t argument */
seq_t a(idx_t n)
{
    return _A((int64_t)n);
}
