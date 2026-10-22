/*
 * A329996  Beatty sequence for x^3, where 1/x^3 + 1/3^x = 1.
 * https://oeis.org/A329996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊n · x³⌋, where 1/x³ + 1/3ˣ = 1 (x ≈ 1.12177497)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ----------  internal state for the constant α = x³  ---------- */
static __int128  const_alpha_num = 0;            /* α scaled by 2⁶³   */
static bool      const_inited = false;

/* Helper: compute the constant α≈1.411…, scaled by 2⁶³, once. */
static void
init_alpha(void)
{
    /* Newton–Raphson to solve 1/x³ + 1/3ˣ = 1. */
    long double x = 1.12L;                      /* good initial guess */
    for (int i = 0; i < 25; ++i) {
        long double fx  = 1.0L/(x*x*x) + powl(3.0L, -x) - 1.0L;
        long double dfx = -3.0L/(x*x*x*x) - logl(3.0L)*powl(3.0L, -x);
        long double dx  = fx/dfx;
        x -= dx;
        if (fabsl(dx) < 1e-30L)
            break;
    }
    long double alpha = x*x*x;                  /* α = x³          */
    const long double SCALE = (long double)((__int128)1 << 63);
    const_alpha_num = (__int128)floorl(alpha * SCALE + 0.5L);   /* nearest integer */
    const_inited = true;
}

/* ----------  sequence a(n)  ---------- */
seq_t
a(idx_t n)
{
    if (!const_inited)
        init_alpha();

    /* floor(n * α) = floor(n * (const_alpha_num / 2⁶³)) */
    __int128 prod = (__int128)n * const_alpha_num;
    __int128 res  = prod >> 63;                 /* division by 2⁶³ */

    return (seq_t)res;
}
