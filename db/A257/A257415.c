/*
 * A257415  Values of n such that there are exactly 8 solutions to x^2 - y^2 =
 *          n with x > y >= 0.
 * https://oeis.org/A257415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257415: values of n such that x^2 – y^2 = n has exactly 8 solutions
   with x > y ≥ 0.  The function a(idx_t n) returns the n‑th such value. */

/*=====================================================================*/
/*  Dependencies: the build supplies the integer types idx_t and seq_t
 *  as well as a few helper headers (see the specification).              */
#include <stdint.h>
#include <stdlib.h>      /* calloc, abort */
#include <stdbool.h>     /* bool, true, false */

/*=====================================================================*/
/*  Parameters                                                            */
/*  MAXN must be larger than the largest n we expect to search for.
 *  20 000 000 comfortably exceeds the 10 000‑th term of the sequence.   */
#ifndef MAXN
#define MAXN 20000000
#endif

/*=====================================================================*/
/*  Global state for divisor-count table                               */
static uint16_t *divcnt = NULL;   /* divisor counts for 1..MAXN          */
static bool divcnt_ready = false;

/*=====================================================================*/
/*  Integer square‑root for 64‑bit values                               */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (n >> 1) + 1, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/*=====================================================================*/
/*  Test for perfect square                                             */
static bool is_square(uint64_t n)
{
    uint64_t r = isqrt_u64(n);
    return ((unsigned __int128)r * r == n);
}

/*=====================================================================*/
/*  Initialise divisor‑count array (once)                               */
static void init_divcnt(void)
{
    if (divcnt_ready) return;
    /* allocate and zero the table */
    divcnt = (uint16_t*)calloc((size_t)MAXN + 1, sizeof(uint16_t));
    if (!divcnt) abort();   /* allocation failed */
    for (uint32_t i = 1; i <= (uint32_t)MAXN; ++i) {
        for (uint32_t j = i; j <= (uint32_t)MAXN; j += i) {
            ++divcnt[j];
        }
    }
    divcnt_ready = true;
}

/*=====================================================================*/
/*  Count number of solutions of x^2 – y^2 = n                         */
static int count_sol(uint64_t n)
{
    /* Only n odd or divisible by 4 can have solutions. */
    if ((n & 1ULL) == 1ULL) {                /* n odd */
        uint64_t d = divcnt[n];
        return is_square(n) ? (int)((d + 1) / 2) : (int)(d / 2);
    }
    if ((n & 3ULL) == 0ULL) {                /* n ≡ 0 (mod 4) */
        uint64_t m = n >> 2;                 /* m = n / 4 */
        uint64_t d = divcnt[m];
        return is_square(m) ? (int)((d + 1) / 2) : (int)(d / 2);
    }
    /* n ≡ 2 (mod 4) has no solutions */
    return 0;
}

/*=====================================================================*/
/*  Main function required by the problem definition                    */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    init_divcnt();                           /* ensure table ready */

    idx_t found = 0;
    uint64_t cand = 1;

    while (found < n) {
        /* 480 is the first element; skip earlier numbers to save time */
        if (cand >= 480) {
            if (count_sol(cand) == 8) {
                ++found;
                if (found == n) return (seq_t)cand;
            }
        }
        ++cand;
    }
    return (seq_t)0;  /* unreachable */
}
