/*
 * A340242  Square array read by upward antidiagonals: T(n,k) is the number of
 *          n-ary strings of length k containing 000.
 * https://oeis.org/A340242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of n‑ary strings of length k that contain the
 *            substring "000", where the pair (n,k) is obtained by reading the
 *            infinite square array T(n,k) upward antidiagonally, with offset
 *            2 (so a(2) is the first term)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 147, u64 n <= 371,
 *            u128 n <= 1067, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for standard integer types */

/* Compute p^e with exponentiation by squaring.  e is a non‑negative
   idx_t, base and result are seq_t.  No overflow is assumed to be
   handled by the seq_t implementation itself. */
static seq_t pow_seq(seq_t base, idx_t e)
{
    seq_t result = 1;
    while (e)
    {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Count the number of a‑ary strings of length b that contain the
   substring "000".  The method is a standard automaton DP that
   counts strings that avoid "000" and subtracts from the total
   number of strings. */
static seq_t count_containing_000(idx_t a, idx_t b)
{
    if (b < 3)
        return 0ULL; /* technically unreachable because b >= 3 in the table */

    /* States: S0 = ends with non‑zero,
              S1 = ends with one zero,
              S2 = ends with two zeros. */
    seq_t S0 = 1;
    seq_t S1 = 0;
    seq_t S2 = 0;
    const seq_t mult = (seq_t)(a - 1);   /* number of non‑zero symbols */

    for (idx_t i = 0; i < b; ++i)
    {
        seq_t new0 = (S0 + S1 + S2) * mult; /* next symbol is non‑zero */
        seq_t new1 = S0;                    /* next symbol is the first zero */
        seq_t new2 = S1;                    /* next symbol is the second zero */

        S0 = new0;
        S1 = new1;
        S2 = new2;
    }

    seq_t avoid = S0 + S1 + S2;          /* strings without "000" */
    seq_t total = pow_seq((seq_t)a, b); /* total strings a^b */
    return total - avoid;
}

/* Main sequence function.  It maps the (n‑indexed) item to the
   corresponding (alphabet size, string length) pair using the
   upward antidiagonal traversal and then calls the DP routine. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0; /* behaviour below the offset does not matter */

    /* Zero‑based index within the antidiagonal ordering. */
    idx_t idx = n - 2;

    /* Find t such that t*(t+1)/2 > idx.  Here t = s-4 where
       s = n + k (the diagonal sum). */
    idx_t t = 0;
    while (((t * (t + 1)) / 2) <= idx)
        ++t;

    idx_t s = t + 4;                     /* current diagonal sum n + k */
    idx_t prev = ((t - 1) * t) / 2;      /* number of terms before this diagonal */
    idx_t pos = idx - prev;              /* position within the diagonal */

    /* Diagonal is traversed from larger alphabet size to smaller:
       n_max = s - 3, n_min = 2.  pos = 0 corresponds to n_max. */
    idx_t alphabet = (s - 3) - pos;      /* alphabet size n */
    idx_t length   = s - alphabet;       /* string length k */

    return count_containing_000(alphabet, length);
}
