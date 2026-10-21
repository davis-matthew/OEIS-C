/*
 * A285806  Square array A(m,n) = number whose binary expansion is the
 *          concatenation of those of { m, m+1, ..., m+n }, with m, n >= 1,
 *          read by falling antidiagonals.
 * https://oeis.org/A285806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenates the binary expansions of the integers
   m, m+1, …, m+n (where n is the second array index) and
   then lists those values following the falling antidiagonals
   of the square array A(m,n) (m,n ≥ 1).  The first output
   (a(1)) is A(1,1).  Each term a(n) is obtained by
   determining the antidiagonal in which the pair (m,n) lies
   and performing the binary concatenation.  This implementation
   works for any unsigned integer or arbitrary‑precision
   library supplied as seq_t, with external types seq_t and
   idx_t already defined.  The only required operations are
   integer arithmetic, bitwise shifts, and comparison. */

#include <stdint.h>   /* for uint64_t on builds that need it */


/* Return the number of bits necessary to represent x
   (x > 0).  Works for any seq_t that supports >> and ==. */
static unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Concatenate the binary expansion of b after a. */
static seq_t concat_bits(seq_t a, seq_t b)
{
    unsigned blen = bitlen_seq_t_local(b);
    return (a << blen) | b;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* out‑of‑range, offset starts at 1 */
        return 0;

    /* Find the antidiagonal index D such that the cumulative
       number of terms up to that diagonal is at least n.
       The cumulative count after D diagonals is D(D+1)/2. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit arithmetic to avoid overflow of mid*(mid+1) */
        if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t D = low;                           /* diagonal number (1‑based) */
    idx_t before = ((unsigned __int128)(D - 1) * D) / 2; /* terms before D */

    idx_t pos_in_diag = n - before;          /* 1‑based position inside diagonal */
    idx_t m = pos_in_diag;                   /* starting integer (row index) */
    idx_t n_part = D + 1 - pos_in_diag;      /* second array index */

    /* Concatenate the binary expansions of m, m+1, …, m + n_part */
    seq_t val = 0;
    idx_t end = m + n_part;                  /* inclusive */
    for (idx_t x = m; x <= end; ++x) {
        seq_t xi = (seq_t)x;
        val = concat_bits(val, xi);
    }
    return val;
}
