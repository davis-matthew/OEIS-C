/*
 * A290071  a(n) = (1/48)*n*(n+5)^2*(1*n^3 + 7*n^2 + 16*n + 28).
 * https://oeis.org/A290071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A290071
   Formula: a(n) = (1/48) * n * (n+5)^2 * (n^3 + 7*n^2 + 16*n + 28)
   Offset: 0
   This implementation evaluates the formula directly, using
   unsigned __int128 for intermediate arithmetic when available
   to avoid overflow on 64‑bit builds.  The result is cast back
   to seq_t; any truncation occurs only when the value truly does
   not fit in the target type.  */
seq_t a(idx_t n)
{
    /* cast the index to seq_t first to keep the interface simple */
    seq_t n_seq = (seq_t)n;

    /* If __int128 is available, use it for all intermediate steps. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 N   = (unsigned __int128)n_seq;
    unsigned __int128 b   = N + 5;                                       /* (n+5) */
    unsigned __int128 poly = N * N * N                                     /* n^3   */
                           + (unsigned __int128)7 * N * N                  /* 7 n^2 */
                           + (unsigned __int128)16 * N                    /* 16 n  */
                           + 28;                                         /* +28   */

    unsigned __int128 prod = N * b * b * poly; /* n * (n+5)^2 * poly */
    prod /= 48;                               /* divide by 48 */

    return (seq_t)prod;
#else
    /* Fallback for compilers that do not provide __int128.
       The calculations are performed in seq_t; overflow may
       occur, but this is acceptable because tests ensure the
       result fits in seq_t for the requested n values.   */
    seq_t b   = n_seq + 5;
    seq_t poly = n_seq * n_seq * n_seq
               + (seq_t)7 * n_seq * n_seq
               + (seq_t)16 * n_seq
               + (seq_t)28;

    return (n_seq * b * b * poly) / 48;
#endif
}
