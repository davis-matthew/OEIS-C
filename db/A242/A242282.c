/*
 * A242282  a(n) = Sum_{k=0..n} (k!)^4 * StirlingS2(n,k)^2.
 * https://oeis.org/A242282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^4 * StirlingS2(n,k)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  ----------  Required types  ----------  */
/*  seq_t and idx_t are supplied by the build system.  */
/*  We use them only as specified:  */
/*   - idx_t is an unsigned integer (e.g., uint64_t)  */
/*   - seq_t is an unsigned integer type of width 8/16/32/64 or 128 bits,  */
/*     or an arbitrary‑precision integer.  The arithmetic on seq_t behaves  */
/*     like that of unsigned integers (truncating on overflow).  */

/*  ----------  Implementation  ----------  */

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 directly.  */
    if (n == 0)
        return 1;

    /* Allocate arrays for factorials and Stirling numbers.  */
    size_t sz = (size_t)n + 1;                 /* size_t is large enough  */
    seq_t *fac = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *S   = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fac || !S) {
        /* If allocation fails, return 0 as a safe sentinel.  */
        free(fac);
        free(S);
        return 0;
    }

    /* Initialise arrays.  */
    fft_init:
    for (size_t i = 0; i < sz; ++i) {
        fac[i] = 0;
        S[i]   = 0;
    }

    fac[0] = 1;         /* 0! = 1  */
    S[0]   = 1;         /* S(0,0) = 1  */

    /* Compute factorials up to n.  */
    for (size_t i = 1; i <= (size_t)n; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;

    /* Compute Stirling numbers of the second kind using the recurrence */
    /*   S(m,k) = k * S(m-1,k) + S(m-1,k-1)  */
    for (size_t m = 1; m <= (size_t)n; ++m) {
        /* Update in reverse so that S[k-1] still holds the previous row. */
        for (size_t k = m; k >= 1; --k)
            S[k] = k * S[k] + S[k - 1];
        S[0] = 0;       /* S(m,0) = 0 for m > 0 */
    }

    /* Compute the sum over k.  */
    seq_t sum = 0;
    for (size_t k = 1; k <= (size_t)n; ++k) {
        seq_t f   = fac[k];
        seq_t f2  = f * f;     /* (k!)^2  */
        seq_t f4  = f2 * f2;   /* (k!)^4  */
        seq_t s   = S[k];
        seq_t s2  = s * s;     /* S(n,k)^2 */
        sum += f4 * s2;
    }

    /* Free allocated memory.  */
    free(fac);
    free(S);
    return sum;
}
