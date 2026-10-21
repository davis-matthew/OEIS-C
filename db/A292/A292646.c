/*
 * A292646  Rank of e*n when all the numbers e*j and (e+1)*k, for j>=1, k>=1,
 *          are jointly ranked.
 * https://oeis.org/A292646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor( n*e/(e+1) ) (e = 2.718281828459045… )
 * Range:     u8 n <= 147, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static const unsigned long long D_CONST = 1ULL << 61;   /* 2^61 ~ 2.3e18  */
static unsigned long long P_ALPHA = 0;                 /* floor(e/(e+1) * D_CONST) */
static int P_INITIALIZED = 0;

static void init_alpha(void)
{
    if (!P_INITIALIZED) {
        /* long double has > 19 decimal digits precision on common platforms */
        long double alpha = 2.71828182845904523536L / 3.71828182845904523536L;
        P_ALPHA = (unsigned long long)floorl(alpha * (long double)D_CONST);
        P_INITIALIZED = 1;
    }
}

/* Return the OEIS A292646 term at index n (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */
    init_alpha();
    unsigned __int128 prod = (unsigned __int128)n * P_ALPHA;
    unsigned long long floor_artist = (unsigned long long)(prod / D_CONST);
    return (seq_t)n + (seq_t)floor_artist;
}
