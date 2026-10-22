/*
 * A339949  a(n) is the greatest runlength in all n-sections of the infinite
 *          Fibonacci word A014675.
 * https://oeis.org/A339949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows Joshi's algorithm for the run‑length in the
 *            n‑sections * of the Fibonacci word. The implementation avoids
 *            floating point by using * a fixed‑point representation of φ with
 *            60 fractional bits
 * Range:     u8 n <= 232, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* 60‑bit fractional scale (≈1.15e18) */
    static const unsigned __int128 SCALE = (unsigned __int128)1 << 60;

    /* Pre‑compute constants once */
    static const unsigned __int128 SQRT5 = (unsigned __int128)
        ( sqrtl((long double)5.0) * (long double)SCALE );
    static const unsigned __int128 PHI = (SCALE + SQRT5) >> 1;            /* φ * SCALE */
    static const unsigned __int128 PHI_MINUS1 = (SQRT5 - SCALE) >> 1;     /* (φ-1) * SCALE  */
    static const unsigned __int128 THRESH = ((unsigned __int128)3 * SCALE - SQRT5) >> 1; /* (3-√5)/2 * SCALE */

    /* Guard against n == 0 (offset is 1, behaviour below does not matter) */
    if (n == 0) return (seq_t)0;

    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 nPHI = n128 * PHI;
    unsigned __int128 frac = nPHI % SCALE;
    unsigned __int128 g = frac <= SCALE - frac ? frac : SCALE - frac;  /* g(n) * SCALE */

    unsigned __int128 result;
    if (g <= THRESH) {
        /* a(n) = ceil( (φ‑1) / g ) */
        result = (PHI_MINUS1 + g - 1) / g;
    } else {
        /* Compute g(2n) * SCALE */
        unsigned __int128 n2PHI = (n128 << 1) * PHI;
        unsigned __int128 frac2 = n2PHI % SCALE;
        unsigned __int128 g2 = frac2 <= SCALE - frac2 ? frac2 : SCALE - frac2;

        unsigned __int128 numer = PHI_MINUS1 - g;          /* (φ‑1 – g) * SCALE */
        unsigned __int128 ceildiv = (numer + g2 - 1) / g2; /* ceil((φ‑1 – g)/g(2n)) */
        result = ceildiv * 2;
    }
    return (seq_t)result;
}
