/*
 * A245159  Number of length n 0..4 arrays with new values introduced in order
 *          from both ends.
 * https://oeis.org/A245159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-3375 - 525*2^(4+n) - 1300*3^n + 3*5^n * +
 *            100*(297+9*2^(2+n)+2*3^n)*n) / 43200 * with a(1) = 1 (formula
 *            holds for n≥2)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 33, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------- Utility: exponentiation --------- */
/* For small seq_t (≤64‑bit) use 128‑bit intermediates */
static inline unsigned __int128 pow_uint128(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* For arbitrary‑precision seq_t */
static inline seq_t pow_uint_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* a(1) is defined explicitly */
    if (n == 1) return (seq_t)1;

    /* Branch on the width of seq_t */
    if (sizeof(seq_t) <= 8) {                       /* 8‑bit up to 64‑bit types */
        unsigned __int128 p2_n4 = pow_uint128((unsigned __int128)2, n + 4);
        unsigned __int128 p2_n2 = pow_uint128((unsigned __int128)2, n + 2);
        unsigned __int128 p3_n  = pow_uint128((unsigned __int128)3, n);
        unsigned __int128 p5_n  = pow_uint128((unsigned __int128)5, n);

        unsigned __int128 neg   = (unsigned __int128)3375
                                 + (unsigned __int128)525 * p2_n4
                                 + (unsigned __int128)1300 * p3_n;

        unsigned __int128 inner = (unsigned __int128)297
                                 + (unsigned __int128)9 * p2_n2
                                 + (unsigned __int128)2 * p3_n;

        unsigned __int128 pos   = (unsigned __int128)3 * p5_n
                                 + (unsigned __int128)100 * inner * (unsigned __int128)n;

        unsigned __int128 numer = pos - neg;

        return (seq_t)(numer / (unsigned __int128)43200);
    } else { /* seq_t is 128‑bit or arbitrary precision */
        seq_t p2_n4 = pow_uint_seq((seq_t)2, n + 4);
        seq_t p2_n2 = pow_uint_seq((seq_t)2, n + 2);
        seq_t p3_n  = pow_uint_seq((seq_t)3, n);
        seq_t p5_n  = pow_uint_seq((seq_t)5, n);

        seq_t neg   = (seq_t)3375
                     + (seq_t)525 * p2_n4
                     + (seq_t)1300 * p3_n;

        seq_t inner = (seq_t)297
                     + (seq_t)9 * p2_n2
                     + (seq_t)2 * p3_n;

        seq_t pos   = (seq_t)3 * p5_n
                     + (seq_t)100 * inner * (seq_t)n;

        seq_t numer = pos - neg;

        return numer / (seq_t)43200;
    }
}
