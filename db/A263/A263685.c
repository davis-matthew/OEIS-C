/*
 * A263685  Number of inequivalent placements of n nonattacking rooks on n X n
 *          board up to rotations of the board.
 * https://oeis.org/A263685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Let m = ⌊n/4⌋. If n ≡ 0 or 1 (mod 4) a(n) = [ n! + (2m)!·2^(2m)
 *            + 2·((2m)!/m!) ] / 4 If n ≡ 2 or 3 (mod 4) a(n) = [ n! +
 *            (2m+1)!·2^(2m+1) ] / 4 (a(0)=a(1)=1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/factorial.h"


seq_t a(idx_t n)
{
    /* Handle the very small cases directly. */
    if (n <= 1) return (seq_t)1;

    idx_t m = n / 4;

/* ---------- 64‑bit optimisation using 128‑bit intermediate  --------- */
#if defined(__SIZEOF_INT128__) && defined(_MSC_VER) == 0
    /* Use the built‑in 128‑bit type only when the target type is 64‑bit
       and we are probing values that still fit inside 128 bits.  */
    if (sizeof(seq_t) == 8 && n <= 21) {
        typedef unsigned __int128 u128;

        /* n! */
        u128 fact_n = 1;
        for (idx_t i = 2; i <= n; ++i) fact_n *= (u128)i;

        if ((n & 3) == 2 || (n & 3) == 3) {                /* n ≡ 2,3 (mod 4) */
            idx_t t = 2 * m + 1;                           /* (2m+1) */
            u128 fact_t = 1;
            for (idx_t i = 2; i <= t; ++i) fact_t *= (u128)i;
            u128 pow2_val = ((u128)1) << (t);              /* 2^(2m+1) */
            u128 sum = fact_n + fact_t * pow2_val;
            return (seq_t)(sum >> 2);                      /* divide by 4 */
        }
        /* n ≡ 0 or 1 (mod 4) */
        idx_t t1 = 2 * m;
        u128 fact_t1 = 1;
        for (idx_t i = 2; i <= t1; ++i) fact_t1 *= (u128)i;

        u128 pow2_val = ((u128)1) << (t1);                /* 2^(2m) */

        idx_t t2 = m;
        u128 fact_t2 = 1;
        for (idx_t i = 2; i <= t2; ++i) fact_t2 *= (u128)i;

        u128 part1 = fact_t1 * pow2_val;
        u128 part2 = (fact_t1 / fact_t2) * ((u128)2);
        u128 sum   = fact_n + part1 + part2;
        return (seq_t)(sum >> 2);                          /* divide by 4 */
    }
#endif

/* ----------  General big‑integer computation (works for all other seq_t) --------- */
    seq_t fact_n = factorial(n);

    if ((n & 3) == 2 || (n & 3) == 3) {                    /* n ≡ 2,3 (mod 4) */
        idx_t t = 2 * m + 1;                              /* (2m+1) */
        seq_t fact_t = factorial(t);
        seq_t term = fact_t * pow2(t);                     /* (2m+1)!·2^(2m+1) */
        seq_t sum  = fact_n + term;
        return sum / (seq_t)4;
    }

    /* n ≡ 0 or 1 (mod 4) */
    seq_t fact_2m   = factorial(2 * m);
    seq_t pow2_val  = pow2(2 * m);                          /* 2^(2m) */
    seq_t part1     = fact_2m * pow2_val;                   /* (2m)!·2^(2m) */
    seq_t fact_m    = factorial(m);
    seq_t part2     = (fact_2m / fact_m) * ((seq_t)2);      /* 2·((2m)!/m!) */
    seq_t sum       = fact_n + part1 + part2;
    return sum / (seq_t)4;
}
