/*
 * A277638  Binomial partial sums of sequence A007004.
 * https://oeis.org/A277638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277638 – "Binomial partial sums of sequence A007004" 
   (OEIS Formula:  a(n) = Sum_{k=0..n} binomial(n,k)*multinomial(k,k,k)/(k+1))
   Recurrence (for n >= 3):
     n*(n+1)*a(n) = 2*(3n-1)*(5n-3)*a(n-1)
                     - (n-1)*(57n-56)*a(n-2)
                     + 28*(n-2)*(n-1)*a(n-3)
   Initial values:
     a(0)=1, a(1)=4, a(2)=37
   The routine below evaluates the recurrence iteratively and
   requires no additional memory beyond a few 64‑bit or arbitrary‑precision
   words.  It works for any seq_t implementation that supports the
   standard integer operators.  If the compiler supports unsigned __int128
   it is used for the intermediate arithmetic of 64‑bit seq_t types;
   otherwise the computation falls back to seq_t arithmetic.

   No global cache is kept – the value is recomputed independently on
   each call, which is fast enough for n up to 10 000. */

#include <stddef.h>   /* For size_t (needed only for the sizeof test) */

#ifdef __SIZEOF_INT128__
#  define USE_INT128 1
#else
#  define USE_INT128 0
#endif

/* Compute a(n) by the recurrence.  For n < 3 the values are
   returned directly.  For larger n the calculation proceeds
   from n = 3 upward, keeping only the last three terms.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)37;

    seq_t a0 = (seq_t)1;     /* a(n-3) */
    seq_t a1 = (seq_t)4;     /* a(n-2) */
    seq_t a2 = (seq_t)37;    /* a(n-1) */
    seq_t an = 0;

#if USE_INT128
    /* 128‑bit helper – used when the compiler supports unsigned __int128. */
    for (idx_t i = 3; i <= n; ++i) {
        unsigned __int128 N  = (unsigned __int128)i;
        unsigned __int128 A0 = (unsigned __int128)a0;
        unsigned __int128 A1 = (unsigned __int128)a1;
        unsigned __int128 A2 = (unsigned __int128)a2;

        unsigned __int128 part1 = 2 * ((3*N - 1) * (5*N - 3)) * A2;
        unsigned __int128 part2 = (N - 1) * (57*N - 56) * A1;
        unsigned __int128 part3 = 28 * (N - 2) * (N - 1) * A0;
        unsigned __int128 num   = part1 - part2 + part3;
        unsigned __int128 denom = N * (N + 1);

        an = (seq_t)(num / denom);

        a0 = a1;
        a1 = a2;
        a2 = an;
    }
#else
    /* Fallback that uses seq_t arithmetic only.  This works for
       arbitrary‑precision seq_t types and also for uint64_t when
       a 128‑bit temporary is unavailable. */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t N = (seq_t)i;
        seq_t part1 = 2 * ((3 * N - 1) * (5 * N - 3)) * a2;
        seq_t part2 = (i - 1) * (57 * N - 56) * a1;
        seq_t part3 = 28 * ((i - 2) * (i - 1)) * a0;
        seq_t num   = part1 - part2 + part3;
        seq_t denom = N * (N + 1);

        an = num / denom;

        a0 = a1;
        a1 = a2;
        a2 = an;
    }
#endif

    return an;
}
