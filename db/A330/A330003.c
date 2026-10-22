/*
 * A330003  Beatty sequence for (x+1)^2, where 1/x + 1/(x+1)^2 = 1.
 * https://oeis.org/A330003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * y ), where y = (x + 1)^2 * and 1/x + 1/(x+1)^2
 *            = 1 (x ≈ 1.246979603717467…). * y satisfies the same recurrence
 *            as the Beatty sequence * for (x + 1)^2. * * The value y is
 *            computed exactly in integer arithmetic using a * scaled rational
 *            approximation of the root x, thus no floatin...
 * Range:     u8 n <= 50, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define SCALE ((unsigned long long)1000000000000ULL) /* 10^12 scaling factor */

/* static state for the scaled root and its square */
static unsigned long long X_scaled = 0;          /* floor(x * SCALE)   */
static unsigned __int128 Ynum = 0;          /* (X_scaled + SCALE)^2   */
static unsigned __int128 Yden = 0;          /* SCALE^2                */
static bool initialized = false;

/* initialise the scaled root and the square used in the Beatty formula */
static void init(void)
{
    if (initialized) return;

    unsigned long long low = SCALE;         /* 1 * SCALE */
    unsigned long long high = 2 * SCALE;    /* 2 * SCALE */

    /* binary search for floor(root * SCALE) */
    while (low < high)
    {
        unsigned long long mid = (low + high + 1) >> 1; /* upper mid */

        /* evaluate g(mid) = mid^3 + mid^2*SCALE - 2*mid*SCALE^2 - SCALE^3 */
        signed __int128 mid3   = (signed __int128)mid * mid * mid;
        signed __int128 mid2S  = (signed __int128)mid * mid * SCALE;
        signed __int128 midS2  = (signed __int128)mid * SCALE * SCALE;
        signed __int128 g      = mid3 + mid2S - 2 * midS2
                           - (signed __int128)SCALE * SCALE * SCALE;

        if (g > 0)
            high = mid - 1;      /* mid is too large, overshoot root */
        else
            low = mid;           /* mid is <= root * SCALE */
    }

    X_scaled = low;                                 /* floor(x * SCALE) */
    unsigned long long xp = X_scaled + SCALE;       /* x + 1 scaled   */
    Ynum = (unsigned __int128)xp * xp;             /* (x + 1)^2 * SCALE^2  */
    Yden = (unsigned __int128)SCALE * SCALE;       /* SCALE^2             */
    initialized = true;
}

/* return the n-th element of sequence A330003 */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* compute floor( n * Ynum / Yden ) using 128‑bit arithmetic  */
    unsigned __int128 prod = (unsigned __int128)n * Ynum;
    seq_t result = (seq_t)(prod / Yden);

    return result;
}
