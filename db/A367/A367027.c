/*
 * A367027  G.f. A(x) satisfies A(x) = 1 + x*A(x)^3 - x^2*A(x)^5.
 * https://oeis.org/A367027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 18, i64 n <= 33, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367027
 *
 * Functional equation:  A(x) = 1 + x*A(x)^3 - x^2*A(x)^5
 *
 * Recurrence (see OEIS entry):
 *   -50*(5*n+6)*(5*n+2)*(5*n+3)*(5*n+4)*a(n)
 *   + 5*(3041*n^4+15872*n^3+32443*n^2+31300*n+12096)*a(n+1)
 *   - 4*(2*n+5)*(n+2)*(331*n^2+1025*n+1044)*a(n+2)
 *   + 36*(2*n+5)*(n+3)*(n+2)*(2*n+7)*a(n+3) = 0
 *
 * Initial values: a(0)=1, a(1)=1, a(2)=2
 *
 * The following implementation memoises the sequence and uses a
 * 128‑bit intermediate type when seq_t is at most 64‑bit to avoid
 * overflow.  For bigger seq_t types (arbitrary precision) all
 * arithmetic is performed with seq_t itself (the build provides
 * operator overloads for these types).  The recurrence is
 * evaluated exactly; every division is exact by definition.
 */

#include <stdlib.h>

static seq_t *cached_seq = NULL;
static size_t cached_size = 0; /* number of elements actually stored */
static size_t cached_cap  = 0; /* allocated capacity */

/* Computes a(n) using the recurrence, extending the cache if needed. */
static inline seq_t compute_a(idx_t n)
{
    if (n < cached_size)
        return cached_seq[n];

    /* Ensure capacity covers n+1 elements. */
    if ((size_t)n + 1 > cached_cap) {
        size_t new_cap = cached_cap ? cached_cap : 8;
        while ((size_t)n + 1 > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cached_seq, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failure – unlikely in tests */
            return 0;
        cached_seq = tmp;
        cached_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cached_size; i <= n; ++i) {
        idx_t r = i - 3;                 /* recurrence index n = r          */
        seq_t a0 = cached_seq[r];
        seq_t a1 = cached_seq[r + 1];
        seq_t a2 = cached_seq[r + 2];
        seq_t a3;

        if (sizeof(seq_t) <= sizeof(long long)) {   /* 64‑bit or smaller */
            __int128 rn   = (__int128)r;
            __int128 P0 = (5*rn + 6) * (5*rn + 2) * (5*rn + 3) * (5*rn + 4);
            __int128 P1 = (((( (__int128)3041*rn + (__int128)15872)) * rn + (__int128)32443) * rn + (__int128)31300) * rn + 12096;
            __int128 P2 = (2*rn + 5) * (rn + 2) * (331*rn*rn + 1025*rn + 1044);
            __int128 Q  = (2*rn + 5) * (rn + 3) * (rn + 2) * (2*rn + 7);
            __int128 num = 50*P0*( (__int128)a0 )
                         - 5*P1*( (__int128)a1 )
                         + 4*P2*( (__int128)a2 );
            __int128 den = 36*Q;
            a3 = (seq_t)(num / den);
        } else {                        /* arbitrary precision */
            seq_t rn   = (seq_t)r;
            seq_t P0 = 5*rn + 6;
            P0 *= 5*rn + 2;
            P0 *= 5*rn + 3;
            P0 *= 5*rn + 4;

            seq_t P1 = 3041;
            P1 = P1*rn + 15872;
            P1 = P1*rn + 32443;
            P1 = P1*rn + 31300;
            P1 *= rn;
            P1 += 12096;

            seq_t P2 = (2*rn + 5) * (rn + 2) *
                       (331*rn*rn + 1025*rn + 1044);

            seq_t Q  = (2*rn + 5) * (rn + 3) * (rn + 2) * (2*rn + 7);

            seq_t num = 50*P0*a0 - 5*P1*a1 + 4*P2*a2;
            seq_t den = 36*Q;
            a3 = num / den;            /* division exact by construction */
        }

        cached_seq[i] = a3;
    }

    cached_size = (size_t)n + 1;
    return cached_seq[n];
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (cached_seq == NULL) {
        cached_cap = 8;
        cached_seq = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cached_seq)
            return 0;                 /* allocation failed – unlikely */
        cached_seq[0] = (seq_t)1;
        cached_seq[1] = (seq_t)1;
        cached_seq[2] = (seq_t)2;
        cached_size = 3;
    }
    return compute_a(n);
}
