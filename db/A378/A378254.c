/*
 * A378254  Cogrowth sequence of the 20-element group C10 X C2 = <S,T | S^10,
 *          T^2, [S,T]>.
 * https://oeis.org/A378254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (recurrence form):
    Let q(x) = (4x-1)(25x^4+10x^2-5x+1)(x^4+4x^3+6x^2-x+1)
             = -1 +10x -45x^2 +120x^3 -210x^4 +254x^5 -120x^6 +540x^7 +375x^8 +100x^9
    Then a(n) satisfies
        a(n) = 10*a(n-1) – 45*a(n-2) + 120*a(n-3) – 210*a(n-4)
              + 254*a(n-5) – 120*a(n-6) + 540*a(n-7) + 375*a(n-8) + 100*a(n-9)
    for n >= 10, with initial values
        a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1,
        a(5)=2, a(6)=67, a(7)=1002, a(8)=8009, a(9)=43759.
    The implementation memoises the sequence up to the largest n
    requested, so later calls are O(1). */

#include <stdlib.h>
#include <string.h>

/* external linkage ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static memoisation of already computed values */
    static seq_t *seq = NULL;     /* array of computed terms */
    static size_t len = 0;        /* number of terms stored       */
    static size_t cap = 0;        /* current allocation size       */

    /* first 10 terms, used as initial conditions */
    static const seq_t init[10] = {
        /* a(0) … a(9) */
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)67, (seq_t)1002, (seq_t)8009, (seq_t)43759
    };

    /* initialise cache on first call */
    if (seq == NULL) {
        cap = 32;                                 /* start capacity   */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (seq == NULL) return (seq_t)0;        /* OOM: return 0    */
        memcpy(seq, init, sizeof(init));
        len = 10;                                 /* number of terms stored */
    }

    /* resize if needed (typical growth by powers of 2) */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (tmp == NULL) return seq[n % len];    /* OOM: return existing value  */
        seq = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n if not yet present */
    if (len <= (size_t)n) {
        /* recurrence coefficients q1 … q9 (see comment above) */
        const long long coeffs[9] = {
            10, -45, 120, -210, 254, -120, 540, 375, 100
        };

        while (len <= (size_t)n) {
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 9; ++k) {
                seq_t term = seq[len - 1 - k];
                long long c   = coeffs[k];
                seq_t mult    = term * (seq_t)(c >= 0 ? c : -c);
                if (c >= 0)
                    pos += mult;
                else
                    neg += mult;
            }
            /* q0 = -1, so division by -1 gives a(n) = pos - neg */
            seq[len] = pos - neg;
            ++len;
        }
    }

    return seq[n];
}
